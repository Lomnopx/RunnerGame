// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunnerGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Kismet/KismetStringLibrary.h"
#include "Runtime/Core/Public/Misc/CString.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// ARunnerGameCharacter

ARunnerGameCharacter::ARunnerGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ARunnerGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	Speed = 2.0f;
	Score = 0;
	MaxLives = 3;
	CurrentLives = MaxLives;
	LoadScoresFromFile();
}

void ARunnerGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Call your function every frame
	if (!Gameover)
	{
		Move(FVector2D(0, Speed));
		GetCharacterMovement()->MaxWalkSpeed += 5 * DeltaTime;
		Score += GetCharacterMovement()->MaxWalkSpeed * DeltaTime;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARunnerGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunnerGameCharacter::Move);

		//Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARunnerGameCharacter::Look);

	}

}

void ARunnerGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(RightDirection, MovementVector.X*2);
		AddMovementInput(ForwardDirection, Speed/2);
	}
}

void ARunnerGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARunnerGameCharacter::SaveTopScores()
{
	TopScores.Add(Score);
	TopScores.Sort([](const int32& A, const int32& B) { return A > B; });
	TopScores.SetNum(FMath::Min(TopScores.Num(), 5));
	Gameover = true;
}

int32 ARunnerGameCharacter::GetScore() const
{
	return Score;
}

void ARunnerGameCharacter::GameOver()
{
	GetCharacterMovement()->MaxWalkSpeed = 0;
	SaveTopScores();
}

bool ARunnerGameCharacter::SaveScoresToFile(FString SaveDirectory, FString FileName, TArray<FString> SaveText, bool AllowOverWriting = false)
{
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}
	FString FinalString = "";
	for (int i = 0; i < 5; i++)
	{
		if (!SaveText.IsValidIndex(i))//If list is empty put 0
		{
			FinalString += "0";
			FinalString += LINE_TERMINATOR;
		}
		else
		{
			FinalString += SaveText[i];
			FinalString += LINE_TERMINATOR;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalString);
	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);
}

void ARunnerGameCharacter::LoadScoresFromFile()
{
	
	FString SaveFilePath = "F:\\Unreal Repository\\RunnerGame\\HighScoreFile.txt";

	// Read the text file into a string
	FString ScoresString;
	FFileHelper::LoadFileToString(ScoresString, *SaveFilePath);
	
	// Split the string by newline to get individual scores
	TArray<FString> ScoreStrings;
	ScoresString.ParseIntoArray(ScoreStrings, TEXT("\n"), true);

	// Convert the strings to integers and update the array
	TopScores.Empty();
	int32 TempScore=0;
	for (int i = 0; i < 5; i++)
	{
		if (!ScoreStrings.IsValidIndex(i))//If list is empty put 0
		{
			TempScore = -1;
		}
		else
		{
			TempScore = FCString::Atoi(*ScoreStrings[i]);
		}
		TopScores.Add(TempScore);
	}

	
}

void ARunnerGameCharacter::TakeDamage()
{
	CurrentLives--;
	if (CurrentLives <= 0)
	{
		GameOver();
	}
}







