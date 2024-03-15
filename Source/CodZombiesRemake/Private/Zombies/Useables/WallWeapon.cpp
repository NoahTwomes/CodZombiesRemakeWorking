// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/WallWeapon.h"
#include "Zombies/Useables/WeaponBase.h"
#include "Player/CharacterBase.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

AWallWeapon::AWallWeapon()
{
	WallWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = WallWeaponMesh;

	Cost = 1000;
	bIsUsed = false;
}

void AWallWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWallWeapon, bIsUsed);
}

void AWallWeapon::OnRep_WeaponPurchased()
{

}

void AWallWeapon::Use(ACharacterBase* Player)
{
	if (HasAuthority() && Player)
	{

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Player;
		if (AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnParams))
		{
			//Weapon->SetActorLocation(Player->GetActorLocation());
			Player->GivePlayerWeapon(Weapon);
			UE_LOG(LogTemp, Warning, TEXT("SPAWNED WEAPON"));
		}
	}

}
