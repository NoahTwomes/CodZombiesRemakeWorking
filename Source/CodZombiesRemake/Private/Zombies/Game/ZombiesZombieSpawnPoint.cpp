// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Game/ZombiesZombieSpawnPoint.h"
#include "Zombies/Useables/Barricade.h"

AZombiesZombieSpawnPoint::AZombiesZombieSpawnPoint()
{
	Zone = 0;
	bIsActive = false;
}

void AZombiesZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

}

ABarricade* AZombiesZombieSpawnPoint::GetLinkedBarricade()
{
	return LinkedBarricade;
}

uint8 AZombiesZombieSpawnPoint::GetZone()
{
	return Zone;
}

void AZombiesZombieSpawnPoint::SetZone(uint8 ZoneNumber)
{
	Zone = ZoneNumber;
}

bool AZombiesZombieSpawnPoint::IsActive()
{
	return bIsActive;
}

void AZombiesZombieSpawnPoint::activate()
{
	bIsActive = true;
}

