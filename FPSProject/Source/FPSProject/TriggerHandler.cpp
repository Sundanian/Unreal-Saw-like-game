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

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Event Triggered"));

	//Should be calling GetTotalMassOnTriggerVolume()

	GetTotalMassOnTriggerVolume();
}


// Called every frame
void UTriggerHandler::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!DoorTrigger) return;

	AActor * Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	float TotalMass = 0.0f;
	TArray<AActor *> OverlappingActors;
	DoorTrigger->GetOverlappingActors(OverlappingActors);
	for (AActor *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		TotalMass += 55;
	}

	if (TotalMass > 50.0f) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OPEN DOOR"));
		OpenDoor.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Event triggered"));

		GetOwner()->SetActorRotation(FRotator(0, 180, 0));
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


