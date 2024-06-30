// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/InteractableBase.h"
#include "MysteryBox.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AMysteryBox : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AMysteryBox();

protected:
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class USkeletalMeshComponent* MysteryBoxMesh;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class USkeletalMeshComponent* TESTWEAPONMESHLOCATION;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UStaticMeshComponent* CollisionMesh;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimationAsset* OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimationAsset* CloseAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FVector BottomLocation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FVector TopLocation;
	FVector CurrentLocation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	uint16 Cost;

	UPROPERTY(Replicated)
	bool bCanUseBox;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	TArray<TSubclassOf<class AWeaponBase>> WeaponClasses;

	UPROPERTY(BlueprintReadOnly, Category = "Zombies Settings")
	TArray<class AWeaponBase*> BoxWeaponArray;
	
	FTimerHandle TRaiseWeapon;
	FTimerHandle TLowerWeapon;

protected:
	virtual void BeginPlay() override;

	void AbleToGrabWeapon();
	void RaiseWeapon();
	void LowerWeapon();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_BoxUsed(uint8 RandomIndex);
	bool Multi_BoxUsed_Validate(uint8 RandomIndex);
	void Multi_BoxUsed_Implementation(uint8 RandomIndex);

public:
	virtual FString GetUIMessage(class ACharacterBase* Player) override;

	virtual void Use(class ACharacterBase* Player) override;
};
