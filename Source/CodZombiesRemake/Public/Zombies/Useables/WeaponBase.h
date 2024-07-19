// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"


UENUM(BlueprintType)
enum EHitLocation
{
	None UMETA(DisplayName = "None"),
	Head UMETA(DisplayName ="Head"),
	Torso UMETA(DisplayName = "Torso")
};

USTRUCT(BlueprintType)
struct FWeaponDamage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float BaseDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly)
	float HeadMultiplier = 3.5f;
	UPROPERTY(EditDefaultsOnly)
	float TorsoMultiplier = 1.1f;
	float GetDamage(EHitLocation HitLocation)
	{
		if (HitLocation == EHitLocation::Head)
			return BaseDamage * HeadMultiplier;
		else if (HitLocation == EHitLocation::Torso)
			return BaseDamage * TorsoMultiplier;
		else
			return BaseDamage;
		
	}
};

UENUM(BlueprintType)
enum EWeaponID
{
	Colt1911 UMETA(DisplayName = "1911"),
	M1Carbine UMETA(DisplayName = "M1Carbine"),
	STG44 UMETA(DisplayName = "STG44")
};

UCLASS()
class CODZOMBIESREMAKE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	TEnumAsByte<EWeaponID> WeaponID;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimMontage* FPSArmsFireMontage;
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimMontage* ThirdPersonMontage;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	int32 MagazineMaxAmmo;

	FTimerHandle WeaponFireHandle;

	UPROPERTY(Replicated)
	int32 CurrentTotalAmmo;
	UPROPERTY(Replicated)
	int32 CurrentMagazineAmmo;

	UPROPERTY()
	int i;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FHitResult> PerformLineTrace(class ACharacterBase* ShootingPlayer);
	TArray<FHitResult> PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation);
	void Loop();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(const TArray<FHitResult>& HitResults);
	bool Server_Fire_Validate(const TArray<FHitResult>& HitResults);
	virtual void Server_Fire_Implementation(const TArray<FHitResult>& HitResults);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Fire(const FHitResult& HitResult);
	bool Multi_Fire_Validate(const FHitResult& HitResult);
	virtual void Multi_Fire_Implementation(const FHitResult& HitResult);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Reload();
	bool Server_Reload_Validate();
	virtual void Server_Reload_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Reload();
	bool Multi_Reload_Validate();
	virtual void Multi_Reload_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_RefillAmmo();
	void Client_RefillAmmo_Implementation();
	
private:
	bool bIsFiring;


public:	
	virtual bool Fire(class ACharacterBase* ShootingPlayer);
	virtual void StopFiring();
	FWeaponDamage GetWeaponDamage();
	FWeaponDamage GetUpgradedWeaponDamage();
	virtual bool IsFiring() { return false; }

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FWeaponDamage WeaponDamage;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FWeaponDamage UpgradedWeaponDamage;

	virtual bool Reload();

	UFUNCTION(BlueprintCallable)
	void RefillAmmo();

	bool IsTotalAmmoFull() { return CurrentTotalAmmo == WeaponMaxAmmo; ;}

	TArray<int32> GetCurrentAmmo();

	class UAnimMontage* GetFireAnimMontage();
	
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWeaponID> GetWeaponID();
	void WeaponIsNowInHand(bool InHand);
	class USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

};
