// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/Perk1.h"
#include "Player/CharacterBase.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include <Player/ZombiesPlayerState.h>

APerk1::APerk1()
{
	PerkMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = PerkMachineMesh;

	Cost = 1000;
	bIsUsed = false;
	PerkUIMessage = "Press F To Buy ";
}

void APerk1::BeginPlay()
{

}

void APerk1::PurchasePerk(ACharacterBase* Player)
{
	bIsUsed = true;
}

FString APerk1::GetUIMessage(ACharacterBase* Player)
{
	if (bIsUsed == false)
	{
		return PerkUIMessage;
	}
	return UIMessage;
}

void APerk1::Use(ACharacterBase* Player)
{

	if (HasAuthority() && Player && bIsUsed == false)
	{
		Player->MaxHealth = 250;
		
	}

	if (bIsUsed == false)
	if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
		if (!PState->DecrementPoints(Cost) && bIsUsed == false)
		{
			bIsUsed = true;
		}


	
			return;


}
