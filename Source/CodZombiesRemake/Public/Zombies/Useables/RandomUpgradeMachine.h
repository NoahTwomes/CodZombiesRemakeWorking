// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/InteractableBase.h"
#include "RandomUpgradeMachine.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API ARandomUpgradeMachine : public AInteractableBase
{
	GENERATED_BODY()

public:
	ARandomUpgradeMachine();

protected:
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UStaticMeshComponent* PerkMachineMesh;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	uint16 Cost;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FString PerkUIMessage;
	
protected:
	virtual void BeginPlay() override;


public:
	virtual FString GetUIMessage(class ACharacterBase* Player) override;
	UFUNCTION(BlueprintCallable)
	virtual void Use(class ACharacterBase* Player) override;

	void ability1(class ACharacterBase* Player);
	void ability2(class ACharacterBase* Player);
	void ability3(class ACharacterBase* Player);
};
