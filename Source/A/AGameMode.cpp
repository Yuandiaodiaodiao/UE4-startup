// Copyright Epic Games, Inc. All Rights Reserved.

#include "AGameMode.h"
#include "ACharacter.h"
#include "MyPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AAGameMode::AAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerStateClass=AMyPlayerState::StaticClass();
}
