// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

ATank::ATank()
{
	SpringComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	// GetController() stores a APlayerController object in a AController pointer, cast it to assign successfully.
	TankPlayerController = Cast<APlayerController>(GetController());
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,	// Used whenever performing a linetrace.
			false,			// Determines whether to use simple or complex collision molds.
			HitResult);		// Local variable that will be populated with the hit result.
		
		// Debug for mouse location.
		/*DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			100.f,
			12,
			FColor::Emerald,
			false,
			-1.f);*/
		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);			// Hides actor in game.

	SetActorTickEnabled(false);

	bAlive = false;
}

void ATank::Move(float Value)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
	AddActorLocalOffset(DeltaLocation, true);		// Moves RootComponent based on given FVector. 2nd parameter sweeps for any collision.
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnSpeed;
	AddActorLocalRotation(DeltaRotation, true);		// Moves RootComponent based on given FRotator.
}

APlayerController* ATank::GetTankPlayerController() const
{
	return TankPlayerController;
}