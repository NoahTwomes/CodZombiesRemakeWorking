// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ZombiesPlayerSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AZombiesPlayerSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	AZombiesPlayerSpawnPoint();

protected:
	bool bIsUsed;

public:
	bool IsUsed();
	void SetUsed(bool Used);

};
