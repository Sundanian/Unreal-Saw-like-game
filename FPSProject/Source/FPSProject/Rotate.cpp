// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "Rotate.h"


// Sets default values for this component's properties
URotate::URotate()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotate::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URotate::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );	
		
}

