// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/RandomUpgradeMachine.h"
#include "Player/CharacterBase.h"
#include "Zombies/Useables/WeaponBase.h"
#include "CharacterMovementComponent.generated.h"

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
	if (Player->upgraded == false)
	{
		return PerkUIMessage;
	}
	return UIMessage;
}

void ARandomUpgradeMachine::Use(ACharacterBase* Player)
{
	
	if (Player->upgraded == false)
	if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())

		if (!PState->DecrementPoints(Cost))

			Player->upgraded = true;




	Player->upgraded = true;

			return;

}

void ARandomUpgradeMachine::ability1(ACharacterBase* Player)
{
	Player->WalkSpeed += 50.0f;
	Player->RunSpeed += 50.0f;
	UE_LOG(LogTemp, Warning, TEXT("Ability 1"));
}

void ARandomUpgradeMachine::ability2(ACharacterBase* Player)
{
	Player->upgraded = true;
	UE_LOG(LogTemp, Warning, TEXT("Damage Increased"));
}

void ARandomUpgradeMachine::ability3(ACharacterBase* Player)
{
	Player->MaxStamina += 50;
	UE_LOG(LogTemp, Warning, TEXT("Ability 3"));
}

void ARandomUpgradeMachine::ability4(ACharacterBase* Player)
{
	Player->MaxStamina -= 50;
	UE_LOG(LogTemp, Warning, TEXT("Ability 4"));
}


