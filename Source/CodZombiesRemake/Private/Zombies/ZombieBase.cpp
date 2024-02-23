// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/ZombieBase.h"
#include "Player/CharacterBase.h"
#include "Player/ZombiesPlayerState.h"

// Sets default values
AZombieBase::AZombieBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieBase::Hit(ACharacterBase* Player)
{
	if (Player)
	{
		if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
		{
			PState->IncrementPoints(100);
		}
	}
}




