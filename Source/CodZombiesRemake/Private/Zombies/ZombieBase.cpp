// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/ZombieBase.h"
#include "Player/CharacterBase.h"

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
	if (HasAuthority() && Player)
	{
		Player->IncrementPoints(100);
	}
}




