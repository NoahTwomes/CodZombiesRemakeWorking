// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Game/ZombiesGameState.h"
#include <Net/UnrealNetwork.h>
#include "Zombies/ZombieBase.h"
#include <Player/ZombiesPlayerState.h>
#include <Player/CharacterBase.h>
#include <Kismet/GameplayStatics.h>

AZombiesGameState::AZombiesGameState()
{
	RoundNumber = 1;
	HudNumber = 1;
	ZombiesOnMap = 0;
	TotalZombiesRemaining = 5;
	ZombieHealth = 150;
	NewSpeed = 500.0f;
	HasIncreased = false;
}

uint16 AZombiesGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombiesGameState::IncrementRoundNumber()
{
	++RoundNumber;
	++HudNumber;
	HasIncreased = true;

	UGameplayStatics::PlaySound2D(this, SoundToPlay, 8.0f, 8.0f, 0.0f);

	if (RoundNumber < 10)
	{
		ZombieHealth += 100;
	}
	else
	{
		ZombieHealth *= 1.1f;
	}

	if (RoundNumber >= 7)
	{
		
		NewSpeed = 800.0f;

		
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

float AZombiesGameState::GetZombieSpeed()
{
	return NewSpeed;
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

void AZombiesGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AZombiesGameState, HudNumber);

}

int32 AZombiesGameState::CurrentRound()
{
	return RoundNumber;
}
