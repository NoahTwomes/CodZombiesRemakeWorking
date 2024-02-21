// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "ZombiesZombieSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AZombiesZombieSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	AZombiesZombieSpawnPoint();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class ABarricade* LinkedBarricade;


	uint8 Zone;
	bool bIsActive;

	virtual void BeginPlay() override;

public:
	class ABarricade* GetLinkedBarricade();
	uint8 GetZone();
	void SetZone(uint8 ZoneNumber);
	bool IsActive();
	void activate();
};
