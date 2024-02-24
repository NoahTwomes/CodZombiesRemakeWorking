// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/ZombieBase.h"
#include "Player/CharacterBase.h"
#include "Player/ZombiesPlayerState.h"
#include "Zombies/Game/ZombiesGameMode.h"

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
	
}


void AZombieBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombieBase, bIsDead);
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
}

uint8 AZombieBase::GetHitPart(FString BoneName)
{
	if (BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{ //limbHit

		
		return 1;
	}
	else if (BoneName.Contains(FString("spine")) || BoneName.Contains(FString("pelvis")))
	{ //torsoHit
		
		return 2;
	}
	else if (BoneName.Equals(FString("neck_01")))
	{ //Neck Hit
		
		return 3;
	}
	else if (BoneName.Equals(FString("head")))
	{ //Head hit
		
		return 4;
	}
	//Nothing hit
	return 0;
}


uint8 AZombieBase::GetPointsForHit(uint8 HitPart)
{
	if (Health - 50 <= 0)
	{
		switch (HitPart)
		{
		case 1: {DecrementHealth(50); return 50; }
		  case 2: {DecrementHealth(50); return 60; }
		  case 3: {DecrementHealth(50); return 70;}
		  case 4: {DecrementHealth(50); return 100; }
		  default: {return 0; }
		}
	}

	else
	{
		DecrementHealth(50);
		return 10;
	}

}

void AZombieBase::Hit(ACharacterBase* Player,FHitResult HitResult)
{
	if (Player)
	{
		if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
		{
			
			FString BoneName = HitResult.BoneName.ToString();
				if (BoneName == FString("None"))
					return;
				if (uint8 HitPart = GetHitPart(BoneName))
				{
					if (uint8 PointsForHit = GetPointsForHit(HitPart))
					{
					
						PState->IncrementPoints(PointsForHit);
					}
					
				}
			
		}
	}
}




