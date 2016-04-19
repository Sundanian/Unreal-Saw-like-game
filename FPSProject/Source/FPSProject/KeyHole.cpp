// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "KeyHole.h"


// Sets default values
AKeyHole::AKeyHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM"));

	SM->OnComponentHit.AddDynamic(this, &AKeyHole::OnHit);
}

void AKeyHole::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	if (OtherActor->Tags.Contains(TEXT("Key")))
	{
				GLog->Log(TEXT("The Game Should End Now. TBI"));
	}

	//GLog->Log(*OtherActor->GetName());
	//ConstructorHelpers::FObjectFinder<UBlueprint> BluePrintObj(TEXT("Blueprint'/Game/Blueprints/BP_Key.BP_Key'"));
	//if (BluePrintObj.Succeeded())
	//{
	//	if (OtherActor->GetClass() == BluePrintObj.Object->GetClass())
	//	{
	//		GLog->Log(TEXT("Success"));
	//	}
	//}
}

// Called when the game starts or when spawned
void AKeyHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyHole::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

