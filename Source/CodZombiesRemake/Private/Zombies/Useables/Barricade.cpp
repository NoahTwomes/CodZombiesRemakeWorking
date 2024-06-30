// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/Barricade.h"
#include "Player/CharacterBase.h"
#include "Player/ZombiesPlayerState.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include <Zombies/Game/ZombiesGameMode.h>

ABarricade::ABarricade()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComp;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	CollisionMesh->SetupAttachment(MeshComp);
	
	Cost = 500;
	AccessZone = 0;

	bIsUsed = false;

	
}


void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABarricade::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABarricade, bIsUsed);
}

void ABarricade::OnRep_BarricadeUsed()
{
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (OpenAnimation)
	MeshComp->PlayAnimation(OpenAnimation, false);
}


void ABarricade::Use(ACharacterBase* Player)
{
	if (HasAuthority() && Player && Player)
	{
		if (AZombiesPlayerState* PState = Player->GetPlayerState<AZombiesPlayerState>())
		{
			if (!PState->DecrementPoints(Cost))
				return;

			UE_LOG(LogTemp, Warning, TEXT("IN USE FUNCTION FOR %s"), *GetName());
			bIsUsed = true;
			OnRep_BarricadeUsed();

			if (AZombiesGameMode* GM = GetWorld()->GetAuthGameMode<AZombiesGameMode>())
			{
				GM->NewZoneActive(AccessZone);
			}
		}
	}
}

uint8 ABarricade::GetAccessZone()
{
	return AccessZone;
}
