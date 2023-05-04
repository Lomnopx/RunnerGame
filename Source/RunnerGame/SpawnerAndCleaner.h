// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "RunnerPlatform.h"
#include "SpawnerAndCleaner.generated.h"


UCLASS()
class RUNNERGAME_API ASpawnerAndCleaner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerAndCleaner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

    UPROPERTY(EditAnywhere, Category = "Spawning")
        TSubclassOf<class AEnemy> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawning")
        TSubclassOf<class ARunnerPlatform> PlatformClass;

    UPROPERTY(EditAnywhere, Category = "Spawning")
        float SpawnDistance = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Cleaning")
        float CleanDistance = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
        bool IgnoreNextSpawn;
    
    void SpawnObjects();
    void CleanObjects();

    FVector PlayerLocation;
    FVector SpawnerLocation;
    
};
