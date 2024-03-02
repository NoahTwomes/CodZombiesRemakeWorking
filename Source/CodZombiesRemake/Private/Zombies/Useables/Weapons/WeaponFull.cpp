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
}



void AWeaponFull::BeginPlay()
{
	Super::BeginPlay();
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
	

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
		GetWorld()->GetTimerManager().SetTimer(WeaponFireHandle, this, &AWeaponFull::PlayWeaponEffects, 0.5f, true);
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



void AWeaponFull::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{

}



bool AWeaponFull::Fire(ACharacterBase* ShootingPlayer)
{
	if (!bIsFiring)
	{
		bIsFiring = true;
		Server_StartFullAutoFire(bIsFiring);
	}

	return true;
}

void AWeaponFull::StopFiring()
{
	UE_LOG(LogTemp, Warning, TEXT("STOP FIRING"));
	bIsFiring = false;
	Server_StartFullAutoFire(bIsFiring);
}


