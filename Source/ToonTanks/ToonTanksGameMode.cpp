// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		TargetTowers--;

		if (TargetTowers == 0)
		{
			GameOver(true);
		}
	}
	FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AToonTanksGameMode::BeginPlay);
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));		// Fetches pawn at player index 0.
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		
		// Here we create a timer that calls SetPlayerEnabledState(true) from ToonTanksPlayerController.
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true		// Function input.
		);
		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false
		);
	}
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Towers;

	// Find all class of type Tower in the world.
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);

	return Towers.Num();
}