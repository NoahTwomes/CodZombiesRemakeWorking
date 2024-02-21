// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Game/ZombiesPlayerSpawnPoint.h"

AZombiesPlayerSpawnPoint::AZombiesPlayerSpawnPoint()
{
	bIsUsed = false;
}

bool AZombiesPlayerSpawnPoint::IsUsed()
{
	return bIsUsed;
}

void AZombiesPlayerSpawnPoint::SetUsed(bool Used)
{
	bIsUsed = Used;
}
