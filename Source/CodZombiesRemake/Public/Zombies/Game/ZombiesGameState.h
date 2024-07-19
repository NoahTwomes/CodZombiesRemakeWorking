// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ZombiesGameState.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AZombiesGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AZombiesGameState();

protected:
	uint16 RoundNumber; //set to replicate
	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 HudNumber;
	uint8 ZombiesOnMap; //Set to replicate
	uint16 TotalZombiesRemaining; //Set to replicate
	float ZombieHealth;
	float NewSpeed;
	UPROPERTY(BlueprintReadWrite)
	bool HasIncreased;

public:
	uint16 GetRoundNumber();
	void IncrementRoundNumber();
	void SetTotalZombiesRemaining(const uint16& ZombieCount);
	uint16 GetTotalZombiesRemaining();
	void ZombieKilled();
	uint8 GetZombiesOnMap();
	void ZombieSpawned();
	float GetZombieHealth();
	float GetZombieSpeed();
	UFUNCTION(BlueprintCallable)
	bool RoundHasIncreased();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* SoundToPlay;

	UFUNCTION(BlueprintCallable)
	int32 CurrentRound();
};
