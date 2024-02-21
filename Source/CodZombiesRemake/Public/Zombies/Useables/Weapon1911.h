// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/WeaponBase.h"
#include "Weapon1911.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AWeapon1911 : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AWeapon1911();
	virtual  TArray<FHitResult> Fire(class ACharacterBase* ShootingPlayer) override;
	virtual void Reload() override;

};
