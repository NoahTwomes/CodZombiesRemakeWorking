// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZombiesPlayerState.h"


#include "Net/UnrealNetwork.h"

AZombiesPlayerState::AZombiesPlayerState()
{
	Points = 500;
}


void AZombiesPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombiesPlayerState, Points);
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
