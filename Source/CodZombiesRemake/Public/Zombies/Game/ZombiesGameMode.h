// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombiesGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AZombiesGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AZombiesGameMode();

protected:
	class AZombiesGameState* ZombieGameState;

	bool bHasLoadedSpawnPoints;
	TArray<class AZombiesPlayerSpawnPoint*> PlayerSpawnPoints;

	TArray<class AZombiesZombieSpawnPoint*> ActiveZombieSpawnPoints;

	TArray<class AZombiesZombieSpawnPoint*> ZombieSpawnPoints;


	UPROPERTY(EditAnywhere, Category = "ZombiesSettings")
	TSubclassOf<class ACharacterBase> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "ZombiesSettings")
	TSubclassOf<class AZombieBase> ZombieClass;
	
	FTimerHandle TZombieSpawnHandle;


	uint16 ZombiesRemaining;
	

protected:
	void CalculateZombieCount();
	void SpawnZombie();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SetSpawnPoints();

public:
	void NewZoneActive(uint8 ZoneNumber);
};
