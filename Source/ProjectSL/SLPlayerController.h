// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API ASLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASLPlayerController();

protected:
	bool bClickedMouse;
	

protected:
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	void InputClickPressed();
	void InputClickReleased();
	void SetNewDestination(const FVector DestLocation);
	void MoveToMouseCursor();

	//void Attack();

//private:
//	class APawn* MyCharacter;
};
