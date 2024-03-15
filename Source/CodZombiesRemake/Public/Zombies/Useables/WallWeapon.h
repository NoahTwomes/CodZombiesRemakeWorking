// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/InteractableBase.h"
#include "WallWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AWallWeapon : public AInteractableBase
{
	GENERATED_BODY()
public:
	AWallWeapon();

protected:
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UStaticMeshComponent* WallWeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	TSubclassOf<class AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	uint16 Cost;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponPurchased)
	bool bIsUsed;

	UFUNCTION()
	void OnRep_WeaponPurchased();

public:
	virtual void Use(class ACharacterBase* Player) override;

	
};
