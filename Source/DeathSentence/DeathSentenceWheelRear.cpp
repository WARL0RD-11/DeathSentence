// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeathSentenceWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UDeathSentenceWheelRear::UDeathSentenceWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}