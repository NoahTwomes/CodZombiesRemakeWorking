// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZombiesPlayerState.h"


#include "Net/UnrealNetwork.h"

AZombiesPlayerState::AZombiesPlayerState()
{
	Points = 500;
	XP = 0;
	Level = 0;
	XPRequired = 250;
}


void AZombiesPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombiesPlayerState, Points);
	DOREPLIFETIME(AZombiesPlayerState, XP);
}

void AZombiesPlayerState::OnRep_PointsChanged()
{
	OnPointsChanged.Broadcast(Points);
}

int32 AZombiesPlayerState::GetPoints()
{
	return Points;
}

void AZombiesPlayerState::IncrementPoints(uint16 Value)
{

		Points += Value;
		if (HasAuthority())
		OnRep_PointsChanged();
		OnPointsChanged.Broadcast(Points);
		UE_LOG(LogTemp, Warning, TEXT("POINTS: %d"), Points);
	
}

bool AZombiesPlayerState::DecrementPoints(uint16 Value)
{
	
	
		if ((Points - Value) < 0)
			return false;
		else
			Points -= Value;

		OnRep_PointsChanged();
		UE_LOG(LogTemp, Warning, TEXT("POINTS: %d"), Points);
	
	return true;

}

void AZombiesPlayerState::IncrementXP(uint16 Value)
{
	XP += Value;
	//if (HasAuthority())
	UE_LOG(LogTemp, Warning, TEXT("XP: %d"), XP);
}

void AZombiesPlayerState::IncrementLevel()
{
	++ Level;
	if (HasAuthority())
		UE_LOG(LogTemp, Warning, TEXT("XP: %d"), XP);
}

void AZombiesPlayerState::IncrementXPRequired()
{
	XPRequired *= 2;
}
