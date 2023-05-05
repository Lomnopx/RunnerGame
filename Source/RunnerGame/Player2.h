// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player2.generated.h"

UCLASS()
class RUNNERGAME_API APlayer2 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer2();

	UFUNCTION(BlueprintCallable)
		void MoveTowardsLocation(FVector TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpeedModifier;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector CurrentVelocity;

	float Speed = 500;

};
