// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectSLGameModeBase.h"
#include "SLCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "SLPlayerController.h"

AProjectSLGameModeBase::AProjectSLGameModeBase()
{
	DefaultPawnClass = ASLCharacter::StaticClass();
	PlayerControllerClass = ASLPlayerController::StaticClass();
}

//void AProjectSLGameModeBase::BeginPlay()
//{
//	Super::BeginPlay();
//	APlayerStart* PlayerStart = Cast<APlayerStart>(FindPlayerStart_Implementation(PlayerControllerClass.GetDefaultObject(), TEXT("PlayerStart")));
//	if(PlayerStart == nullptr)
//	{
//		LOG_S(Error);
//		return;
//	}
//	PlayerStart->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	PlayerStart->Destroy();
//}
