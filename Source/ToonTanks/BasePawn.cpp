// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);			// Attaches component to root component.

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Project Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::HandleDestruction()
{
	// TODO: sound effects
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	
	TurretMesh->SetWorldRotation(		// Using world space rotation since ToTarget is in world space (we fetched a vector calculation between 2 world positions).
		FMath::RInterpTo(				// Interpolation for Rotation.
			TurretMesh->GetComponentRotation(), 
			LookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			25.f
		)
	);		
}

void ABasePawn::Fire()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(		// Spawns actor in the UWorld instance.
		ProjectileClass, 
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation()
	);

	// Set bullet's parent UActor to current pawn.
	Projectile->SetOwner(this);

	//DrawDebugSphere(
	//	GetWorld(),
	//	ProjectileSpawnPoint->GetComponentLocation(),
	//	5.f,
	//	12,
	//	FColor::Red,
	//	false,
	//	3.f);
}