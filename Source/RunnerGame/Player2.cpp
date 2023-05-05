// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2.h"
#include "Enemy.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/UnrealMathVectorCommon.h"

// Sets default values
APlayer2::APlayer2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayer2::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APlayer2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
    Speed += 5 * DeltaTime;
}

void APlayer2::MoveTowardsLocation(FVector TargetLocation)
{
    FVector Direction = FVector(500, TargetLocation.Y - GetActorLocation().Y, TargetLocation.Z - GetActorLocation().Z).GetSafeNormal();
    CurrentVelocity = Direction * Speed; // Set your own speed
}

