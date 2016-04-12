// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TriggerHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UTriggerHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerHandler();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	UPROPERTY(EditAnywhere)
		ATriggerVolume *DoorTrigger = nullptr;

	UPROPERTY(BlueprintAssignable)
		FTriggerDelegate OpenDoor;

	UPROPERTY(BlueprintAssignable)
		FTriggerDelegate CloseDoor;

	float GetTotalMassOnTriggerVolume();

	UFUNCTION() //Important to make the eventhandler into an UFUNCTION, otherwise UE4 will ignore the call
		void OnBeginOverlap(class AActor* OtherActor);


	
};
