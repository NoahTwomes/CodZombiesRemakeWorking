// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZombiesPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsChanged, int32, OnPointsChanged);

UCLASS()
class CODZOMBIESREMAKE_API AZombiesPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AZombiesPlayerState();
protected:

	UPROPERTY(BlueprintAssignable)
	FPointsChanged OnPointsChanged;

	UPROPERTY(ReplicatedUsing = OnRep_PointsChanged, EditDefaultsOnly) //set to replicate MOVE TO PLAYER STATE WHEN CREATED
		int32 Points;


	UFUNCTION()
	void OnRep_PointsChanged();

public:
	void IncrementPoints(uint16 Value);
	bool DecrementPoints(uint16 Value);

	void IncrementXP(uint16 Value);

	void IncrementLevel();
	void IncrementXPRequired();

	UPROPERTY(Replicated, BlueprintReadWrite)
	float XP;

	UPROPERTY(BlueprintReadWrite)
	int32 Level;

	UPROPERTY(BlueprintReadWrite)
	float XPRequired;

	UFUNCTION(BlueprintCallable)
	int32 GetPoints();
	
};
