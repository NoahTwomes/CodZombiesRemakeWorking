// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/WeaponBase.h"
#include "WeaponSemi.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AWeaponSemi : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponSemi();

protected:
	virtual void BeginPlay() override;
	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults) override;
	virtual void Multi_Fire_Implementation(const FHitResult& HitResult) override;
	

public:
	virtual bool Fire(class ACharacterBase* ShootingPlayer) override;
	virtual bool Reload() override;
	
};
