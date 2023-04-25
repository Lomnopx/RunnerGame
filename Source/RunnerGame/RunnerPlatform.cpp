// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPlatform.h"

// Sets default values
ARunnerPlatform::ARunnerPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARunnerPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunnerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

