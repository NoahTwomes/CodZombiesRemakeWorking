// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/RandomUpgradeMachine.h"
#include "Player/CharacterBase.h"

#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include <Player/ZombiesPlayerState.h>

ARandomUpgradeMachine::ARandomUpgradeMachine()
{
	PerkMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = PerkMachineMesh;

	Cost = 1000;
	PerkUIMessage = "Press F To Buy ";
}

void ARandomUpgradeMachine::BeginPlay()
{
	Super::BeginPlay();
}

FString ARandomUpgradeMachine::GetUIMessage(ACharacterBase* Player)
{
	return PerkUIMessage;
}

void ARandomUpgradeMachine::Use(ACharacterBase* Player)
{
	int32 Min = 0;
	int32 Max = 2;
	int32 RandomInt = FMath::RandRange(Min, Max);

	if (RandomInt == 0)
	{
		ability1(Player);
	}
	else if (RandomInt == 1)
	{
		ability2(Player);
	}
	else if (RandomInt == 2)
	{
		ability3(Player);
	}

	if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())

		if (!PState->DecrementPoints(Cost))

			return;

}

void ARandomUpgradeMachine::ability1(ACharacterBase* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability 1"));
}

void ARandomUpgradeMachine::ability2(ACharacterBase* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability 2"));
}

void ARandomUpgradeMachine::ability3(ACharacterBase* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability 3"));
}
