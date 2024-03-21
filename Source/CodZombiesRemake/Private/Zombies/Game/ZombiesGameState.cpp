// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Game/ZombiesGameState.h"

AZombiesGameState::AZombiesGameState()
{
	RoundNumber = 1;
	ZombiesOnMap = 0;
	TotalZombiesRemaining = 5;
	ZombieHealth = 150;
	HasIncreased = false;
}

uint16 AZombiesGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombiesGameState::IncrementRoundNumber()
{
	++RoundNumber;
	HasIncreased = true;
	if (RoundNumber < 10)
	{
		ZombieHealth += 100;
	}
	else
	{
		ZombieHealth *= 1.1f;
	}

	
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

	--TotalZombiesRemaining;
	--ZombiesOnMap;
}

uint8 AZombiesGameState::GetZombiesOnMap()
{
	UE_LOG(LogTemp, Warning, TEXT("ZOMBIES ON MAP: %d"), ZombiesOnMap);
	return ZombiesOnMap;
}

void AZombiesGameState::ZombieSpawned()
{
	++ZombiesOnMap;
}

float AZombiesGameState::GetZombieHealth()
{
	return ZombieHealth;
}

bool AZombiesGameState::RoundHasIncreased()
{
	if (RoundNumber == 2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int32 AZombiesGameState::CurrentRound()
{
	return RoundNumber;
}
