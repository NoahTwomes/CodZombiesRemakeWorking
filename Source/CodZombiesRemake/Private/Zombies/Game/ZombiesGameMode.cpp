// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Game/ZombiesGameMode.h"
#include "Zombies/Game/ZombiesGameState.h"
#include "Zombies/Game/ZombiesPlayerSpawnPoint.h"
#include "Zombies/Game/ZombiesZombieSpawnPoint.h"
#include "CodZombiesRemake/Public/Player/CharacterBase.h"
#include "Zombies/Useables/Barricade.h"
#include "Zombies/ZombieBase.h"



#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AZombiesGameMode::AZombiesGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	bHasLoadedSpawnPoints = false;

	ZombiesRemaining = 0;


}


void AZombiesGameMode::BeginPlay()
{
	Super::BeginPlay();

	ZombieGameState = GetGameState<AZombiesGameState>();
	CalculateZombieCount();

	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombiesZombieSpawnPoint::StaticClass(), TempActors);

		for (AActor* Actor : TempActors)
		{
			if (AZombiesZombieSpawnPoint* SpawnPoint = Cast<AZombiesZombieSpawnPoint>(Actor))
			{
		
				ZombieSpawnPoints.Add(SpawnPoint);
				if (ABarricade* LinkedBarricade = SpawnPoint->GetLinkedBarricade())
				{
					SpawnPoint->SetZone(LinkedBarricade->GetAccessZone());
					UE_LOG(LogTemp, Warning, TEXT("ZONE NUMBER: %d"), LinkedBarricade->GetAccessZone());

				}
				else
				{
					ActiveZombieSpawnPoints.Add(SpawnPoint);
					UE_LOG(LogTemp, Warning, TEXT("THIS IS WORKING"));
				}
			}
			

		}
		UE_LOG(LogTemp, Warning, TEXT("ZombieSpawns: %d"), ZombieSpawnPoints.Num());

		GetWorld()->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &AZombiesGameMode::SpawnZombie, 2.0f, true);
}

void AZombiesGameMode::CalculateZombieCount()
{
	if (ZombieGameState)
	{
		uint16 RoundNumber = ZombieGameState->GetRoundNumber();
		//Do calc here
		ZombiesRemaining = 15;
	}
}

void AZombiesGameMode::SetSpawnPoints()
{
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombiesPlayerSpawnPoint::StaticClass(), TempActors);

	for (AActor* Actor : TempActors)
	{
		if (AZombiesPlayerSpawnPoint* SpawnPoint = Cast<AZombiesPlayerSpawnPoint>(Actor))
		{
			PlayerSpawnPoints.Add(SpawnPoint);

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawn Point count: %d"), PlayerSpawnPoints.Num());
	bHasLoadedSpawnPoints = true;
}



void AZombiesGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	if (bHasLoadedSpawnPoints == false)
        SetSpawnPoints();

	for (AZombiesPlayerSpawnPoint* SpawnPoint : PlayerSpawnPoints)
	{
		if (!SpawnPoint->IsUsed())
		{
			FVector SpawnLocation = SpawnPoint->GetActorLocation();
			if (APawn* Pawn = GetWorld()->SpawnActor<APawn>(PlayerClass, SpawnLocation, FRotator::ZeroRotator))
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawned Pawn to possess"));
				NewPlayer->Possess(Pawn);
				SpawnPoint->SetUsed(true);
				return;
			}
		}
	}

}


void AZombiesGameMode::SpawnZombie()
{
	

	if (ZombiesRemaining > 0)
	{
		int RandomIndex = FMath::RandRange(0, ActiveZombieSpawnPoints.Num() - 1);


		if (AZombiesZombieSpawnPoint* SpawnPoint = ActiveZombieSpawnPoints[RandomIndex])
		{
			FVector Loc = SpawnPoint->GetActorLocation();
			FRotator Rot = SpawnPoint->GetActorRotation();

			if (AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Loc, Rot))
			{
				UE_LOG(LogTemp, Warning, TEXT("SPAWNING ZOMBIE"));
				--ZombiesRemaining;
			}
		}
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
	}
}


void AZombiesGameMode::NewZoneActive(uint8 ZoneNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("SETTING ACTIVE ZONE: %d"), ZoneNumber);

	int Control = 0;


	for (int16 x = ZombieSpawnPoints.Num() - 1; x >= 0; --x)
	{
		AZombiesZombieSpawnPoint* SpawnPoint = ZombieSpawnPoints[x];

		UE_LOG(LogTemp, Warning, TEXT("LoopCount: %d"), Control);
		++Control;

		if (SpawnPoint && ZoneNumber == SpawnPoint->GetZone() && !SpawnPoint->IsActive())
		{
			ActiveZombieSpawnPoints.Add(SpawnPoint);
			SpawnPoint->activate();
			ZombieSpawnPoints.RemoveAt(x);
		}
	}
}

