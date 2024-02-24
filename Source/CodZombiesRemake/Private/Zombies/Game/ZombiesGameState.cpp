// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Game/ZombiesGameState.h"

AZombiesGameState::AZombiesGameState()
{
	RoundNumber = 1;
	ZombiesOnMap = 0;
	TotalZombiesRemaining = 5;
}

uint16 AZombiesGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombiesGameState::IncrementRoundNumber()
{
	++RoundNumber;
}

void AZombiesGameState::SetTotalZombiesRemaining(const uint16& ZombieCount)
{
	TotalZombiesRemaining = ZombieCount;
}

uint16 AZombiesGameState::GetTotalZombiesRemaining()
{
	return TotalZombiesRemaining;
}

void AZombiesGameState::ZombieKilled()
{
	UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HAS BEEN KILLED"));

	--TotalZombiesRemaining;
}
