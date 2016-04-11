// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ActionController.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PUZZLEHORROR_API UActionController : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActionController();

	// Called when the game starts
	virtual void BeginPlay() override;

	void OnPrimaryAction();

	void OnSecondaryAction();

	FVector GetLineTraceEnd();

	FVector GetLineTraceStart();

	FVector GetImpulse();

	FHitResult GetLineTraceFirstPhysicsActors();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UInputComponent *InputComponent = nullptr;
	UPhysicsHandleComponent *PhysicsComponent = nullptr;

	UPROPERTY(EditAnywhere)
		float RayReach = 200.0f;

	UPROPERTY(EditAnywhere)
		float ThrowForce = 5000.0f;

};
