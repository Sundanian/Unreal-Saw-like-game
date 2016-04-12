// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	PhysicsComponent = this->FindComponentByClass<UPhysicsHandleComponent>();
	//UE_LOG(LogTemp, Warning, TEXT("PhysicsComponent"));
	if (!PhysicsComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No UPhysicsHandleComponent found"));
		return;
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PhysicsComponent) return;

	if (PhysicsComponent->GrabbedComponent)
		PhysicsComponent->SetTargetLocation(GetLineTraceEnd());
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	//Movement
	InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	//Camera
	InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	//Jump
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);

	//Shoot
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);

	InputComponent->BindAction("PickUp", IE_Pressed, this, &AFPSCharacter::OnPickUp);
	InputComponent->BindAction("FlashLight", IE_Pressed, this, &AFPSCharacter::FlashLight);

}

void AFPSCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}
void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}

AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create a CameraComponent 
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();

	// Position the camera a bit above the eyes
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
	// Allow the pawn to control rotation.
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);         // only the owning player will see this mesh
	FirstPersonMesh->AttachParent = FirstPersonCameraComponent;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// everyone but the owner can see the regular body mesh
	GetMesh()->SetOwnerNoSee(true);

	PhysicsComponent = ObjectInitializer.CreateDefaultSubobject<UPhysicsHandleComponent>(this, TEXT("PhysicsHC"));
}

void AFPSCharacter::OnFire()
{
	if (!PhysicsComponent) return;

	if (PhysicsComponent->GrabbedComponent) {
		PhysicsComponent->GrabbedComponent->AddImpulse(GetImpulse());
		PhysicsComponent->ReleaseComponent();
	}
	else
	{
		// try and fire a projectile
		if (ProjectileClass != NULL)
		{
			// Get the camera transform
			FVector CameraLoc;
			FRotator CameraRot;
			GetActorEyesViewPoint(CameraLoc, CameraRot);
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
			FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
			FRotator MuzzleRotation = CameraRot;
			MuzzleRotation.Pitch += 10.0f;          // skew the aim upwards a bit
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				// spawn the projectile at the muzzle
				AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					// find launch direction
					FVector const LaunchDir = MuzzleRotation.Vector();
					Projectile->InitVelocity(LaunchDir);
				}
			}
		}
	}
}

FVector AFPSCharacter::GetImpulse() {
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerPosition;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);

	return PlayerRotation.Vector() * ThrowForce;
}

void AFPSCharacter::OnPickUp()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnPickUp"));
	if (!PhysicsComponent) return;
	//UE_LOG(LogTemp, Warning, TEXT("PhysicsComponent exist"));

	if (PhysicsComponent->GrabbedComponent) {
		PhysicsComponent->ReleaseComponent();
		//UE_LOG(LogTemp, Warning, TEXT("Release Component"));
		return;
	}

	FHitResult HitResult = GetLineTraceFirstPhysicsActors();


	if (HitResult.GetActor())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s it hit"), *HitResult.GetActor()->GetName());
		PhysicsComponent->GrabComponent(HitResult.GetComponent(), NAME_None, HitResult.GetActor()->GetActorLocation(), true);
		//UE_LOG(LogTemp, Warning, TEXT("PickedUp"));
	}
}

FVector AFPSCharacter::GetLineTraceStart() {
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerPosition;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);

	return PlayerPosition;
}

FVector AFPSCharacter::GetLineTraceEnd() {
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerPosition;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerPosition, PlayerRotation);

	return PlayerPosition + PlayerRotation.Vector() * RayReach;
}

FHitResult AFPSCharacter::GetLineTraceFirstPhysicsActors() {
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, GetLineTraceStart(), GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams("ActionTrance", false, GetOwner()));

	return HitResult;
}

void AFPSCharacter::FlashLight()
{
}