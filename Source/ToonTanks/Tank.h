// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	bool bAlive = true;

	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 10.f;

	APlayerController* TankPlayerController;

	void Move(float Value);

	void Turn(float value);
};
