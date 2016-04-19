// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSProjectile.h"
#include "EnemyBehavior.h"


// Sets default values
AFPSProjectile::AFPSProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AFPSProjectile::AFPSProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Use a sphere as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	//Sets the collision to use the Projectile profile
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
}

void AFPSProjectile::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		// set the projectile's velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void AFPSProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
		//UE_LOG(LogTemp, Warning, TEXT("Fireable"));
		if (OtherActor->Tags.Contains(TEXT("Enemy")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Enemy"));
			OtherActor->FindComponentByClass<UEnemyBehavior>()->Restart();
		}
	}
}