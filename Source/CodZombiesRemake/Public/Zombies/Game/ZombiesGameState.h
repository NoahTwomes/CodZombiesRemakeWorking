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
	uint16 RoundNumber;
	uint8 ZombiesOnMap;

public:
	uint16 GetRoundNumber();
	void IncrementRoundNumber();
};
