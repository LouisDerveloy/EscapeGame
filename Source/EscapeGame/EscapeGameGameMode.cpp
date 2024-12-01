// Copyright Epic Games, Inc. All Rights Reserved.

#include "EscapeGameGameMode.h"
#include "EscapeGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEscapeGameGameMode::AEscapeGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
