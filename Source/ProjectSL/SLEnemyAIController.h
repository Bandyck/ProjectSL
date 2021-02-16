// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SLEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API ASLEnemyAIController : public AAIController
{
	GENERATED_BODY()

public :
	ASLEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private :
	void OnRepeatTimer();

public :
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
