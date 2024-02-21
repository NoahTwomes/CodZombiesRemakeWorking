// Copyright Epic Games, Inc. All Rights Reserved.

#include "CodZombiesRemakeGameMode.h"
#include "CodZombiesRemakeCharacter.h"
#include "Player/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"

ACodZombiesRemakeGameMode::ACodZombiesRemakeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Game/BlueprintClasses/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
