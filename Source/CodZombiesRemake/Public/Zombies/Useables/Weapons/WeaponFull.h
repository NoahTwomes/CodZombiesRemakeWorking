// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/WeaponBase.h"
#include "WeaponFull.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AWeaponFull : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponFull();


protected:
	UPROPERTY(Replicated) //replicated using for effects on clients
	bool bIsFiring;

	UFUNCTION()
	void OnRep_StartFullAutoFire();
	FTimerHandle WeaponFireHandle;
	void PlayWeaponEffects();

protected:
	virtual void BeginPlay() override;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartFullAutoFire(bool IsFiring);
	bool Server_StartFullAutoFire_Validate(bool IsFiring);
	bool Server_StartFullAutoFire_Implementation(bool IsFiring);

	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults) override;


public:
	virtual bool Fire(class ACharacterBase* ShootingPlayer) override;
	virtual void StopFiring() override;
};
