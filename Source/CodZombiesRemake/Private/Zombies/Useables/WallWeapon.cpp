// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/WallWeapon.h"
#include "Zombies/Useables/WeaponBase.h"
#include "Player/CharacterBase.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include <Player/ZombiesPlayerState.h>

AWallWeapon::AWallWeapon()
{
	WallWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = WallWeaponMesh;

	Cost = 1000;
	AmmoCost = 200;
	bIsUsed = false;
	AmmoUIMessage = "Press F To Buy ";
}

void AWallWeapon::BeginPlay()
{
	Super::BeginPlay();
}



void AWallWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWallWeapon, bIsUsed);
}

void AWallWeapon::OnRep_WeaponPurchased()
{

}



FString AWallWeapon::GetUIMessage(ACharacterBase* Player)
{
	for (AWeaponBase* Weapon : *Player->GetWeaponArray())
		if (Weapon)
			if (Weapon->IsA(WeaponClass))
				return AmmoUIMessage;
			
	return UIMessage;
}

void AWallWeapon::PurchaseAmmo(ACharacterBase* Player)
{
	if (HasAuthority() && Player)
	{
		if (Player->GetCurrentWeapon() && Player->GetCurrentWeapon()->IsA(WeaponClass) && !Player->GetCurrentWeapon()->IsTotalAmmoFull())
		{
			UE_LOG(LogTemp, Warning, TEXT("Purchasing Ammo For: %s"), *Player->GetCurrentWeapon()->GetName());

			if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
				if (!PState->DecrementPoints(AmmoCost))
					return;

			Player->GetCurrentWeapon()->RefillAmmo();
			

		}
	}
}

void AWallWeapon::Use(ACharacterBase* Player)
{
	if (HasAuthority() && Player)
	{

		for (AWeaponBase* Weapon : *Player->GetWeaponArray())
			if (Weapon)
				if (Weapon->IsA(WeaponClass))
				{
					PurchaseAmmo(Player);
					return;
				}
			
		if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
			if (!PState->DecrementPoints(Cost))
				return;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Player;
		if (AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams))
		{
			//Weapon->SetActorLocation(Player->GetActorLocation());
			Player->GivePlayerWeapon(Weapon);
			UE_LOG(LogTemp, Warning, TEXT("SPAWNED WEAPON"));
			if (!bIsUsed)
			{
				bIsUsed = true;
				OnRep_WeaponPurchased();
			}
		}
	}

}
