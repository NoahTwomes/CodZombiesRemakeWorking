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

		GetWorld()->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &AZombiesGameMode::SpawnZombie, 2.0f, true);
		GetWorld()->GetTimerManager().UnPauseTimer(TZombieSpawnHandle);
		CalculateZombieCount();
		
		
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
				--ZombiesRemaining;
			}
		}
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
	}
}

void AZombiesGameMode::CalculateZombieCount()
{
	if (ZombieGameState)
	{
		uint16 RoundNumber = ZombieGameState->GetRoundNumber();
		uint8 PlayerCount = ZombieGameState->PlayerArray.Num();
		uint8 MaxZombiesOnMapAtOnce = 24;
		if (PlayerCount > 1)
		
			MaxZombiesOnMapAtOnce += PlayerCount * 6;
		
		UE_LOG(LogTemp, Warning, TEXT("PlayerCount: %d"), PlayerCount);
		if (RoundNumber > 0 && RoundNumber <= 5)
		{
			UE_LOG(LogTemp, Warning, TEXT("ROUNDS 1-5"));
			ZombiesRemaining = FMath::FloorToInt((RoundNumber * 0.2f) * MaxZombiesOnMapAtOnce);
			UE_LOG(LogTemp, Warning, TEXT("ZombiesRemaining: %d"), ZombiesRemaining);
		
		}
		else
		{
			ZombiesRemaining = FMath::FloorToInt((RoundNumber * 0.15f) * MaxZombiesOnMapAtOnce);
		}
		//Do calc here
	
		ZombieGameState->SetTotalZombiesRemaining(ZombiesRemaining);
		GetWorld()->GetTimerManager().UnPauseTimer(TZombieSpawnHandle);
	}
}

void AZombiesGameMode::ZombieKilled()
{
	if (ZombieGameState)
	{
		ZombieGameState->ZombieKilled();
		if (ZombieGameState->GetTotalZombiesRemaining() == 0)
		{
			ZombieGameState->IncrementRoundNumber();
			FTimerHandle TempHandle;
			GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &AZombiesGameMode::CalculateZombieCount, 5.0f, false);
		

		}
		UE_LOG(LogTemp, Warning, TEXT("Zombies Remaining: %d"), ZombieGameState->GetTotalZombiesRemaining());
	}
}

