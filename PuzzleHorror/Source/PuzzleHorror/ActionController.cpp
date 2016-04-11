// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleHorror.h"
#include "ActionController.h"


// Sets default values for this component's properties
UActionController::UActionController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionController::BeginPlay()
{
	Super::BeginPlay();

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>(); //Denne og den under får begge far i et component, på to forskellige måder
																		  //PhysicsComponent = GetOwner()->GetComponentByClass(UPhysicsHandleComponent::StaticClass());
	PhysicsComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("PrimaryAction", IE_Pressed, this, &UActionController::OnPrimaryAction);
		InputComponent->BindAction("SecondaryAction", IE_Pressed, this, &UActionController::OnSecondaryAction);
	}
	if (!PhysicsComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No UPhysicsHandleComponent found"));
		return;
	}

	// ...

}

void UActionController::OnPrimaryAction() {
	if (!PhysicsComponent) return;

	if (PhysicsComponent->GrabbedComponent) {
		PhysicsComponent->ReleaseComponent();
		return;
	}

	FHitResult HitResult = GetLineTraceFirstPhysicsActors();

	if (HitResult.GetActor())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s it hit"), *HitResult.GetActor()->GetName());
		PhysicsComponent->GrabComponent(HitResult.GetComponent(), NAME_None, HitResult.GetActor()->GetActorLocation(), true);
	}
}

void UActionController::OnSecondaryAction() {

	if (!PhysicsComponent) return;
	if (PhysicsComponent->GrabbedComponent)
	{
		PhysicsComponent->GrabbedComponent->AddImpulse(GetImpulse());
		PhysicsComponent->ReleaseComponent();
	}

	FHitResult HitResult = GetLineTraceFirstPhysicsActors();
	if (HitResult.GetActor())
	{

		HitResult.GetComponent()->AddImpulse(GetImpulse());
	}
}

FVector UActionController::GetLineTraceEnd() {
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerPosition;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);

	return PlayerPosition + PlayerRotation.Vector() * RayReach;
}

FVector UActionController::GetLineTraceStart() {
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerPosition;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);

	return PlayerPosition;
}

FVector UActionController::GetImpulse() {
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerPosition;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);

	return PlayerRotation.Vector() * ThrowForce;
}

FHitResult UActionController::GetLineTraceFirstPhysicsActors() {
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, GetLineTraceStart(), GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams("ActionTrance", false, GetOwner()));

	return HitResult;
}

// Called every frame
void UActionController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

