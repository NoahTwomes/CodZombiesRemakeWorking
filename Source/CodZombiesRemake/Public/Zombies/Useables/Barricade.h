// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/InteractableBase.h"
#include "Barricade.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API ABarricade : public AInteractableBase
{
	GENERATED_BODY()
public:
	ABarricade();

protected:

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UStaticMeshComponent* CollisionMesh;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UAnimationAsset* OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	uint16 Cost;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	uint8 AccessZone;

	UPROPERTY(ReplicatedUsing = OnRep_BarricadeUsed)
	bool bIsUsed;

	UFUNCTION()
	void OnRep_BarricadeUsed();

protected:
	virtual void BeginPlay() override;
	virtual void Use(class ACharacterBase* Player) override;

public:
	uint8 GetAccessZone();
};
