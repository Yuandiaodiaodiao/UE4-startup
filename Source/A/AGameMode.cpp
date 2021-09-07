// Copyright Epic Games, Inc. All Rights Reserved.

#include "AGameMode.h"
#include "ACharacter.h"
#include "MyGameStateBase.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AAGameMode::AAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Blueprints/SideScrollerCharacter.SideScrollerCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerStateClass=AMyPlayerState::StaticClass();
	GameStateClass=AMyGameStateBase::StaticClass();
	PlayerControllerClass=AMyPlayerController::StaticClass();
}
