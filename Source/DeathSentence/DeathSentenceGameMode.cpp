// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeathSentenceGameMode.h"
#include "DeathSentencePlayerController.h"

ADeathSentenceGameMode::ADeathSentenceGameMode()
{
	PlayerControllerClass = ADeathSentencePlayerController::StaticClass();
}
