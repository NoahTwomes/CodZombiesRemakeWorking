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


	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeapon1911::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER FIRE FUNTION"));


	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	
		if (HitResults.Num() > 0)
		{
			for (FHitResult Result : HitResults)
			{
				if (AActor* HitActor = Result.GetActor())
				{
					if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
					{
						UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT %s"), *Zombie->GetName());
						if (ACharacterBase* Player = Cast<ACharacterBase>(GetOwner()))
							Zombie->Hit(Player, Result);
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
				FString HitBone = Result.BoneName.ToString();
				UE_LOG(LogTemp, Warning, TEXT("Hit Bone: %s"), *HitBone);
				
				if (AActor* HitActor = Result.GetActor())
				{
					if (AZombieBase* Zombie = Cast<AZombieBase>(Result.GetActor()))
					{
						UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT %s"), *Zombie->GetName());
						Zombie->Hit(ShootingPlayer, Result);
					}


					UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
				}
			}

		}
	}

	else
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
						Zombie->Hit(ShootingPlayer, Result);
					}


					UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
				}
			}

		}
		Server_Fire(HitResults);
	}
	return TArray<FHitResult>();


}

void AWeapon1911::Reload()
{

}
