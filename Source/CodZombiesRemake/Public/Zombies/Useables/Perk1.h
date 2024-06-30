// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombies/Useables/InteractableBase.h"
#include "Perk1.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API APerk1 : public AInteractableBase
{
	GENERATED_BODY()

public:
	APerk1();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	class UStaticMeshComponent* PerkMachineMesh;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	uint16 Cost;

	UPROPERTY(EditAnywhere, Category = "Zombies Settings")
	FString PerkUIMessage;

	UPROPERTY()
	bool bIsUsed;

protected:
	virtual void BeginPlay() override;

	void PurchasePerk(class ACharacterBase* Player);

public:
	virtual FString GetUIMessage(class ACharacterBase* Player) override;
	UFUNCTION(BlueprintCallable)
	virtual void Use(class ACharacterBase* Player) override;
	UFUNCTION(NetMulticast, Reliable)
	void Multi_use(class ACharacterBase* Player);
	bool Multi_use_Validate(class ACharacterBase* Player);
	void Multi_use_Implementation(class ACharacterBase* Player);
	UFUNCTION(Server, Reliable)
	void Server_use(class ACharacterBase* Player);
	bool Server_use_Validate(class ACharacterBase* Player);
	void Server_use_Implementation(class ACharacterBase* Player);
};
