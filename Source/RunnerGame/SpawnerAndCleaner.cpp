// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerAndCleaner.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASpawnerAndCleaner::ASpawnerAndCleaner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnerAndCleaner::BeginPlay()
{
    Super::BeginPlay();
    // Get initial player and spawner location
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            PlayerLocation = PlayerPawn->GetActorLocation();
        }
    }

    SpawnerLocation = PlayerLocation;
}

void ASpawnerAndCleaner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update player and spawner location
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            PlayerLocation = PlayerPawn->GetActorLocation();
        }
    }

    // Spawn objects
    SpawnObjects();

    // Clean objects
    CleanObjects();
}

void ASpawnerAndCleaner::SpawnObjects()
{
    // Check if the player has moved beyond the spawn distance
    float Distance = FVector::Dist(SpawnerLocation, PlayerLocation);
    //UE_LOG(LogTemp, Log, TEXT("spawner then playerlocation: %s %s"), *SpawnerLocation.ToString(), *PlayerLocation.ToString());
    if (Distance > SpawnDistance)
    {
        SpawnerLocation = PlayerLocation;
        // Spawn an enemy
        if (EnemyClass)
        {
            if (IgnoreNextSpawn && 2 >= FMath::RandRange(1, 10))
            {

            }
            else
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnerLocation + FVector(1000.0f, 0, -SpawnerLocation.Z), FRotator::ZeroRotator, SpawnParams);
                IgnoreNextSpawn = false;
            }
        }

        // Spawn a platform
        if (PlatformClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            GetWorld()->SpawnActor<ARunnerPlatform>(PlatformClass, SpawnerLocation + FVector(0.0f, 0.0f, -SpawnerLocation.Z), FRotator::ZeroRotator, SpawnParams);
        }

        // Update the spawner location
        SpawnerLocation = PlayerLocation;
    }
}

void ASpawnerAndCleaner::CleanObjects()
{
    // Get all actors in the world
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);

    // Loop through all actors
    for (AActor* Actor : Actors)
    {
        // Check if the actor is an enemy or a platform
        if (Actor->IsA(AEnemy::StaticClass()) || Actor->IsA(ARunnerPlatform::StaticClass()))
        {
            FVector ActorLocation = Actor->GetActorLocation();

            // Check if the actor is behind the player character based on the clean distance
            if (ActorLocation.X < SpawnerLocation.X - CleanDistance)
            {
                // Destroy the actor
                Actor->Destroy();
            }
        }
    }
}

