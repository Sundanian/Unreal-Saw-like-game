// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "EnemyBehavior.h"


// Sets default values for this component's properties
UEnemyBehavior::UEnemyBehavior()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyBehavior::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();

	// ...

}

void UEnemyBehavior::GoTowards(FVector goal, float DeltaTime) {
	//UE_LOG(LogTemp, Warning, TEXT("Player: %s"), *goal.ToString());
	FVector currentPosition = GetOwner()->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Enemy: %s"), *currentPosition.ToString());
	FVector diff = goal - currentPosition;
	//UE_LOG(LogTemp, Warning, TEXT("Diff: %s"), *diff.ToString());
	FVector newPos = currentPosition + (diff * DeltaTime * speed);
	//UE_LOG(LogTemp, Warning, TEXT("NewPos: %s"), *newPos.ToString());
	GetOwner()->SetActorLocation(newPos);
}

// Called every frame
void UEnemyBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation); //Gets player position and rotation. Mostly here to get position.

	GoTowards(PlayerPosition, DeltaTime);

	// ...
}

