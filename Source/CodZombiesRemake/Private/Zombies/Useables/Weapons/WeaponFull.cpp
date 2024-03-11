// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/Weapons/WeaponFull.h"
#include "Player/CharacterBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include <Zombies/ZombieBase.h>

AWeaponFull::AWeaponFull()
{
	WeaponMaxAmmo = 240;
	MagazineMaxAmmo = 30;
	FireRate = 500;
}



void AWeaponFull::BeginPlay()
{
	Super::BeginPlay();
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
	
	FireRate = 60 / FireRate;
}

void AWeaponFull::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponFull, bIsFiring, COND_SkipOwner);

}

bool AWeaponFull::Server_StartFullAutoFire_Validate(bool IsFiring)
{
	return true;
}

bool AWeaponFull::Server_StartFullAutoFire_Implementation(bool IsFiring)
{
	bIsFiring = IsFiring;
	OnRep_StartFullAutoFire();

	return true;
}

void AWeaponFull::OnRep_StartFullAutoFire()
{
	if (bIsFiring)
	{
		GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFull::PlayWeaponEffects, FireRate, true);
		PlayWeaponEffects();
	}
	else
		GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
}

void AWeaponFull::PlayWeaponEffects()
{

	
		UE_LOG(LogTemp, Warning, TEXT("ONREP PLAYING EFFECTS FOR FULL AUTO WEAPON"));
		if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwner()))
		{
			if (!Character->IsLocallyControlled() && FireAnimation)
			{
				if (UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance())
				{
					if (FPSArmsFireMontage)
					{
						AnimInstance->Montage_Play(FPSArmsFireMontage);
						if (Character->GetIsAiming())
							AnimInstance->Montage_JumpToSection(FName("FireADS"), FPSArmsFireMontage);
						else
							AnimInstance->Montage_JumpToSection(FName("FireHip"), FPSArmsFireMontage);

					}
				}

				WeaponMesh->PlayAnimation(FireAnimation, false);
			}
		}

}



void AWeaponFull::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
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

	}

}

void AWeaponFull::OnClientFire()
{
	if (CurrentMagazineAmmo > 0)
	{


		if (ACharacterBase* ShootingPlayer = Cast<ACharacterBase>(GetOwner()))
		{
			Super::Fire(ShootingPlayer);
			UE_LOG(LogTemp, Warning, TEXT("On Client Fire"));

			if (UAnimInstance* AnimInstance = ShootingPlayer->GetMesh1P()->GetAnimInstance())
			{
				if (FPSArmsFireMontage)
				{
					AnimInstance->Montage_Play(FPSArmsFireMontage);
					if (ShootingPlayer->GetIsAiming())
						AnimInstance->Montage_JumpToSection(FName("FireADS"), FPSArmsFireMontage);
					else
						AnimInstance->Montage_JumpToSection(FName("FireHip"), FPSArmsFireMontage);
				}
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

			if (!GetWorld()->IsServer())
				Server_Fire(HitResults);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
		bIsFiring = false;
		if (!GetWorld()->IsServer())
			Server_StartFullAutoFire(bIsFiring);
	}
}



bool AWeaponFull::Fire(ACharacterBase* ShootingPlayer)
{
	if (!bIsFiring)
	{
		bIsFiring = true;
		OnClientFire();
		GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFull::OnClientFire, FireRate, true);
		if (!GetWorld()->IsServer())
		Server_StartFullAutoFire(bIsFiring);
	}

	return false;
}

void AWeaponFull::StopFiring()
{
	UE_LOG(LogTemp, Warning, TEXT("STOP FIRING"));
	GetWorld()->GetTimerManager().ClearTimer(WeaponFireHandle);
	bIsFiring = false;
	if (!GetWorld()->IsServer())
	Server_StartFullAutoFire(bIsFiring);
}


