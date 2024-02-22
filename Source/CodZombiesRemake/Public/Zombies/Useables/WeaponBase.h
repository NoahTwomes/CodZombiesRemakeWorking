// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

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
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimMontage* FPSArmsFireMontage;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	int32 BaseDamage;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	int32 WeaponMaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	int32 MagazineMaxAmmo;



	int32 CurrentTotalAmmo;
	int32 CurrentMagazineAmmo;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FHitResult> PerformLineTrace(class ACharacterBase* ShootingPlayer);
	TArray<FHitResult> PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
	bool Server_Fire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);
	virtual void Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

public:	
	virtual TArray<FHitResult> Fire(class ACharacterBase* ShootingPlayer);

	virtual void Reload();

	//first element is magazine ammo, second element is total ammo
	TArray<int32> GetCurrentAmmo();

	class UAnimMontage* GetFireAnimMontage();
	

};
