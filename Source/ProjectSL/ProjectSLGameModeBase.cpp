// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectSLGameModeBase.h"
#include "SLCharacter.h"
#include "SLPlayerController.h"

AProjectSLGameModeBase::AProjectSLGameModeBase()
{
	DefaultPawnClass = ASLCharacter::StaticClass();
	PlayerControllerClass = ASLPlayerController::StaticClass();
}
