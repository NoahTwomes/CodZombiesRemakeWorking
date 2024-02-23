// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/ZombieBase.h"
#include "Player/CharacterBase.h"
#include "Player/ZombiesPlayerState.h"

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
	UE_LOG(LogTemp, Warning, TEXT("ONREP_DIE"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
		UE_LOG(LogTemp, Warning, TEXT("Zombie Died"));
		bIsDead = true;
		OnRep_Die();
	}
}

uint8 AZombieBase::GetPointsForKill(FString BoneName)
{
	if (BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{ //limbHit
		
		DecrementHealth(30);
		return 50;
	}
	else if (BoneName.Contains(FString("spine")) || BoneName.Contains(FString("pelvis")))
	{ //torsoHit
		DecrementHealth(50);
		return 60;
	}
	else if (BoneName.Equals(FString("neck_01")))
	{
		DecrementHealth(70);
		return 70;
	}
	else if (BoneName.Equals(FString("head")))
	{
		DecrementHealth(90);
		return 100;
	}
	return 0;
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

			PState->IncrementPoints(GetPointsForKill(BoneName));
		}
	}
}




