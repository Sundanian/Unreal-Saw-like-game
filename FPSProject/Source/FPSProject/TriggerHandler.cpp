// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "TriggerHandler.h"


// Sets default values for this component's properties
UTriggerHandler::UTriggerHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriggerHandler::BeginPlay()
{
	Super::BeginPlay();
	if (!DoorTrigger) {
		UE_LOG(LogTemp, Error, TEXT("No trigger for %s"), *GetOwner()->GetName());
	}
	else {
		//Attache a listener to the ActorBeginOverlap-event
		DoorTrigger->OnActorBeginOverlap.AddDynamic(this, &UTriggerHandler::OnBeginOverlap);
	}

}

///Method matching the signature of OnActorBeginOverlap
void UTriggerHandler::OnBeginOverlap(class AActor* OtherActor) {
	UE_LOG(LogTemp, Warning, TEXT("Event triggered"));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Event Triggered"));

	//Should be calling GetTotalMassOnTriggerVolume()

}


// Called every frame
void UTriggerHandler::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!DoorTrigger) return;


	if (GetTotalMassOnTriggerVolume() > 50.0f) {
		OpenDoor.Broadcast();
	}
	else {
		CloseDoor.Broadcast();
	}
}

float UTriggerHandler::GetTotalMassOnTriggerVolume()
{
	TArray<AActor *> OverlappingActors;
	float TotalMass = 0.0f;

	DoorTrigger->GetOverlappingActors(OverlappingActors);
	for (AActor *Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}


