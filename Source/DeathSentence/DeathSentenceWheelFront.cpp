// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeathSentenceWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UDeathSentenceWheelFront::UDeathSentenceWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}