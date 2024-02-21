// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Game/ZombiesGameState.h"

AZombiesGameState::AZombiesGameState()
{
	RoundNumber = 1;
	ZombiesOnMap = 0;
}

uint16 AZombiesGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AZombiesGameState::IncrementRoundNumber()
{
	++RoundNumber;
}
