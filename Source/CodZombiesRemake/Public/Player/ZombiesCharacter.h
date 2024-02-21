// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "InputActionValue.h"
#include "ZombiesCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CODZOMBIESREMAKE_API AZombiesCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	AZombiesCharacter();
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
};
