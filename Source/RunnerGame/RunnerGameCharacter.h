// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "RunnerGameCharacter.generated.h"



UCLASS(config=Game)
class ARunnerGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	ARunnerGameCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Runnergame")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runnergame")
		bool Gameover;
	UFUNCTION(BlueprintCallable, Category = "Runnergame")
		void GameOver();

	//SCORESYSTEM
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		TArray<int32> TopScores;


	UFUNCTION(BlueprintPure, Category = "Score")
		int32 GetScore() const;
	
	UFUNCTION(BlueprintCallable, Category = "Score")
		static bool SaveScoresToFile(FString SaveDirectory, FString FileName, TArray<FString> SaveText, bool AllowOverWriting);

	UFUNCTION(BlueprintCallable, Category = "Score")
		void LoadScoresFromFile();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void TakeDamage();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds);
	
	void SaveTopScores();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
		int32 MaxLives; // Maximum number of lives

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		int32 CurrentLives; // Current number of lives

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

