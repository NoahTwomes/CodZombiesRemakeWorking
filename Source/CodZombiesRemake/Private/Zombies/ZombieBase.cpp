// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/ZombieBase.h"
#include "Player/CharacterBase.h"
#include "Player/ZombiesPlayerState.h"
#include "Zombies/Game/ZombiesGameMode.h"
#include "Zombies/Game/ZombiesGameState.h"
#include "Zombies/Useables/WeaponBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AZombieBase::AZombieBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Health = 150;
	bIsDead = false;
	CleanupDelay = 5.0f;
}



// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{


		if (AZombiesGameState* GS = GetWorld()->GetGameState<AZombiesGameState>())
		{
			Health = GS->GetZombieHealth();


			UE_LOG(LogTemp, Warning, TEXT("Zombie Health: %f"),Health);
		}
	}
	
}


void AZombieBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombieBase, bIsDead);
	DOREPLIFETIME(AZombieBase, Health);
}


void AZombieBase::DecrementHealth(int16 Damage)
{
	if (HasAuthority())
	{
		Health -= Damage;
		if (Health <= 0)
		{
			Die();
		}
	}

}

void AZombieBase::OnRep_Die()
{

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Ignore);
	GetMesh()->SetSimulatePhysics(true);
	if (HasAuthority())
	{
		if (AController* AIController = GetController<AController>())
		{
			AIController->Destroy();
		}
		FTimerHandle TempHandle;
		GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &AZombieBase::OnCleanup, CleanupDelay, false);
	}
}

void AZombieBase::OnCleanup()
{
	Destroy();
}

void AZombieBase::Die()
{
	if (HasAuthority())
	{
		//get game mode and decrementation
		if (AZombiesGameMode* GM = GetWorld()->GetAuthGameMode<AZombiesGameMode>())
		{
			GM->ZombieKilled();
		}
		bIsDead = true;
		OnRep_Die();
		
	}

	if (ACharacterBase* Player = Cast<ACharacterBase>(GetOwner()))
	{
		XPOnKill(Player);
	}
}

void AZombieBase::XPOnKill(ACharacterBase* Player)
{

}

uint8 AZombieBase::GetHitPart(FString BoneName)
{
	if (BoneName.Contains(FString("L")) || BoneName.Contains(FString("R")))
	{ //limbHit

		
		return 1;
	}
	else if (BoneName.Contains(FString("Spine")) || BoneName.Contains(FString("Pelvis")))
	{ //torsoHit
		
		return 2;
	}
	else if (BoneName.Equals(FString("Neck")))
	{ //Neck Hit
		
		return 3;
	}
	else if (BoneName.Equals(FString("Head")))
	{ //Head hit
		
		return 4;
	}
	//Nothing hit
	return 0;
}


uint8 AZombieBase::GetPointsForHit(uint8 HitPart, float Damage)
{
	if (Health - Damage <= 0)
	{
		switch (HitPart)
		{
		case 1: {DecrementHealth(Damage); return 50; }
		  case 2: {DecrementHealth(Damage); return 60; }
		  case 3: {DecrementHealth(Damage); return 70;}
		  case 4: {DecrementHealth(Damage); return 100; }
		  default: {return 0; }
		}
	}

	else
	{
		DecrementHealth(Damage);
		return 10;
	}

}

void AZombieBase::Hit(ACharacterBase* Player,FHitResult HitResult)
{
	FVector BloodLocation = HitResult.ImpactPoint;

	if (Player)
	{
		if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
		{
			
			FString BoneName = HitResult.BoneName.ToString();
				if (BoneName == FString("None"))
					return;
				if (uint8 HitPart = GetHitPart(BoneName))
				{
					if (AWeaponBase* PlayerWeapon = Player->GetCurrentWeapon())
					{


						EHitLocation HitLocation = EHitLocation::None;
							switch (HitPart)
							{
							case 2: HitLocation = EHitLocation::Torso; break;
							case 3: HitLocation = EHitLocation::Head; break;
							case 4: HitLocation = EHitLocation::Head;  break;

							}
							float WeaponDamage = PlayerWeapon->GetWeaponDamage().GetDamage(HitLocation);
							UE_LOG(LogTemp, Warning, TEXT("WeaponDamgage: %f"), WeaponDamage);

						if (uint8 PointsForHit = GetPointsForHit(HitPart, WeaponDamage))
						{

							PState->IncrementPoints(PointsForHit);
							PState->IncrementXP(PointsForHit);
						}

						if (NS_Blood)
						{
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_Blood, BloodLocation);
						}
						

					}
				}
				if (PState->XP >= PState->XPRequired)
				{
					PState->IncrementLevel();
					PState->IncrementXPRequired();
				}
			
		}
	}
}




