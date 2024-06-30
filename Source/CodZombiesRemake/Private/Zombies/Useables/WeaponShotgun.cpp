// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/WeaponShotgun.h"

#include "Player/CharacterBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include <Zombies/ZombieBase.h>

AWeaponShotgun::AWeaponShotgun()
{
	WeaponName = "Shotgun";
	WeaponMaxAmmo = 90;
	MagazineMaxAmmo = 5;
}

void AWeaponShotgun::BeginPlay()
{
	Super::BeginPlay();
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeaponShotgun::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
	if (CurrentMagazineAmmo > 0)
	{
		Super::Server_Fire_Implementation(HitResults);
		--CurrentMagazineAmmo;


		if (HitResults.Num() > 0)
		{
			for (FHitResult Result : HitResults)
			{
				if (AActor* HitActor = Result.GetActor())
				{
					if (AZombieBase* Zombie = Cast<AZombieBase>(HitActor))
					{

						if (ACharacterBase* Player = Cast<ACharacterBase>(GetOwner()))
							Zombie->Hit(Player, Result);
					}



				}
			}

		}
		if (HitResults.Num() > 0)
			Multi_Fire(HitResults[0]);
		else
			Multi_Fire(FHitResult());
	}
}

void AWeaponShotgun::Multi_Fire_Implementation(const FHitResult& HitResult)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwner()))
	{
		if (!Character->IsLocallyControlled() && FireAnimation)
		{
			if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
			{
				if (ThirdPersonMontage)
				{
					AnimInstance->Montage_Play(ThirdPersonMontage);
					if (Character->GetIsAiming())
						AnimInstance->Montage_JumpToSection(FName("FireADS"), ThirdPersonMontage);
					else
						AnimInstance->Montage_JumpToSection(FName("FireHip"), ThirdPersonMontage);

				}
			}

			WeaponMesh->PlayAnimation(FireAnimation, false);
		}
	}
}

bool AWeaponShotgun::Fire(ACharacterBase* ShootingPlayer)
{
	if (CurrentMagazineAmmo > 0)
	{
		Super::Fire(ShootingPlayer);

		UE_LOG(LogTemp, Warning, TEXT("SHOOTING SHOTGUN"));

		if (FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}

		
		



			TArray<FHitResult> HitResults = PerformLineTraceShotgun(ShootingPlayer);
			if (HitResults.Num() > 0)
			{
				for (FHitResult& Result : HitResults)
				{
					FString HitBone = Result.BoneName.ToString();
					if (AActor* HitActor = Result.GetActor())
					{
						if (AZombieBase* Zombie = Cast<AZombieBase>(HitActor))
						{
							Zombie->Hit(ShootingPlayer, Result);
						}


						UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitActor->GetName());
					}
				}

			}


			if (GetWorld()->IsServer())
			{
				if (HitResults.Num() > 0)
					Multi_Fire(HitResults[0]);
				else
					Multi_Fire(FHitResult());
			}
			else
				Server_Fire(HitResults);
			return true;
		}
		return false;

	

}

bool AWeaponShotgun::Reload()
{
	return Super::Reload();
}
