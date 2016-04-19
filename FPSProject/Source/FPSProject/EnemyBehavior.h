// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "EnemyBehavior.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UEnemyBehavior : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBehavior();

	// Called when the game starts
	virtual void BeginPlay() override;

	void GoTowards(FVector goal, float DeltaTime);
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void Restart();

private:
	FVector InitialLocation;
	APlayerController *PlayerController;
	FVector PlayerPosition;
	FRotator PlayerRotation;
	UPROPERTY(EditAnywhere)
	float speed = 1;
};
