// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombies/Useables/InteractableBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
	ObjectName = "Default";
	UIMessage = "Press F to buy ";
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FString AInteractableBase::GetUIMessage(ACharacterBase* Player)
{
	return UIMessage;
}

void AInteractableBase::Use(class ACharacterBase* Player)
{

}



