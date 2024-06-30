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
	Super::BeginPlay();

		bIsUsed = false;
}

void APerk1::PurchasePerk(ACharacterBase* Player)
{

}

FString APerk1::GetUIMessage(ACharacterBase* Player)
{
	if (Player->HasPerk1 == false)
	{
		return PerkUIMessage;
	}
	return UIMessage;
}

void APerk1::Use(ACharacterBase* Player)
{
	Server_use(Player);

	//if (HasAuthority() && Player && bIsUsed == false)
	//{
	//	Player->MaxHealth = 250;
		
		
	//}

	//if (bIsUsed == false)
	//if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
		//if (!PState->DecrementPoints(Cost) && bIsUsed == false)
		//{
		//	bIsUsed = true;
		//}


	
		//	return;


}

bool APerk1::Multi_use_Validate(ACharacterBase* Player)
{
	return true;
}

void APerk1::Multi_use_Implementation(ACharacterBase* Player)
{
	if (Player->HasPerk1 == false)
	{
		Player->MaxHealth = 250;


	}

	if (Player->HasPerk1 == false)
		if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())

			if (!PState->DecrementPoints(Cost) && Player-> HasPerk1 == false && Player->MaxHealth == 250)
				Player->HasPerk1 = true;
			

		
	
	Player->HasPerk1 = true;


	return;

}

bool APerk1::Server_use_Validate(ACharacterBase* Player)
{
	return true;
}

void APerk1::Server_use_Implementation(ACharacterBase* Player)
{
	Multi_use(Player);
}


