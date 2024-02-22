// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/Weapon1911.h"
#include "Player/CharacterBase.h"


#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include <Zombies/ZombieBase.h>


AWeapon1911::AWeapon1911()
{
	WeaponName = "1911";
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;
	BaseDamage = 100;

	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeapon1911::Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER FIRE FUNTION"));


	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	

		TArray<FHitResult> HitResults = PerformLineTrace(MuzzleLocation, MuzzleRotation);
		if (HitResults.Num() > 0)
		{
			for (FHitResult& Result : HitResults)
			{
				if (AActor* HitActor = Result.GetActor())
				{
					if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
					{
						UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT %s"), *Zombie->GetName());
						if (ACharacterBase* Player = Cast<ACharacterBase>(GetOwner()))
							Zombie->Hit(Player);
					}


					UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
				}
			}

		}

	}


TArray<FHitResult> AWeapon1911::Fire(ACharacterBase* ShootingPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("SHOOTING 1911"));

	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	if (GetWorld()->IsServer())
	{

		TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);
		if (HitResults.Num() > 0)
		{
			for (FHitResult& Result : HitResults)
			{
				if (AActor* HitActor = Result.GetActor())
				{
					if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
					{
						UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT %s"), *Zombie->GetName());
						Zombie->Hit(ShootingPlayer);
					}


					UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
				}
			}

		}
	}

	else
	{
		Server_Fire(WeaponMesh->GetSocketLocation(FName("muzzleSocket")), WeaponMesh->GetSocketRotation(FName("muzzleSocket")));
	}
	return TArray<FHitResult>();


}

void AWeapon1911::Reload()
{

}
