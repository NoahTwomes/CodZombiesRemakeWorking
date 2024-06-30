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

	UPROPERTY(BlueprintReadWrite)
	int PlayersAlive;


	UPROPERTY(EditAnywhere, Category = "ZombiesSettings")
	TSubclassOf<class ACharacterBase> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "ZombiesSettings")
	TSubclassOf<class AZombieBase> ZombieClass;
	
	FTimerHandle TZombieSpawnHandle;


	uint32 ZombiesRemaining;
	

protected:
	void CalculateZombieCount();
	void SpawnZombie();

protected:
	virtual void BeginPlay() override;

	void SetSpawnPoints();

public:
	void NewZoneActive(uint8 ZoneNumber);
	void ZombieKilled();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(AController* Controller);
};
