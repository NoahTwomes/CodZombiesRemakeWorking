// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/Weapons/WeaponSemi.h"
#include "Player/CharacterBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include <Zombies/ZombieBase.h>


AWeaponSemi::AWeaponSemi()
{
	WeaponName = "1911";
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;



}

void AWeaponSemi::BeginPlay()
{
	Super::BeginPlay();
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}



void AWeaponSemi::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
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

void AWeaponSemi::Multi_Fire_Implementation(const FHitResult& HitResult)
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


bool AWeaponSemi::Fire(ACharacterBase* ShootingPlayer)
{
	

	if (CurrentMagazineAmmo > 0)
	{
		Super::Fire(ShootingPlayer);

		UE_LOG(LogTemp, Warning, TEXT("SHOOTING 1911"));

		if (FireAnimation)
		{
			WeaponMesh->PlayAnimation(FireAnimation, false);
		}

	

		TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);
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


bool AWeaponSemi::Reload()
{
	return Super::Reload();
	/*if (CurrentTotalAmmo > 0 && CurrentMagazineAmmo != MagazineMaxAmmo)
	{
		if (ReloadAnimation)
		{
			WeaponMesh->PlayAnimation(ReloadAnimation, false);
		}

		int Difference = MagazineMaxAmmo - CurrentMagazineAmmo;
		if (CurrentTotalAmmo - Difference >= 0)
		{
			CurrentTotalAmmo -= Difference;
			CurrentMagazineAmmo = MagazineMaxAmmo;
		}
		else
		{
			CurrentMagazineAmmo += CurrentTotalAmmo;
			CurrentTotalAmmo = 0;
		}
	
		return true;
	}
	else
		return false;*/

	
}

