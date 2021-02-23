// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SLSpiderBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API ASLSpiderBossAIController : public AAIController
{
	GENERATED_BODY()

public :
	ASLSpiderBossAIController();
	virtual void OnPossess(APawn* InPawn) override;

public :
	static const FName TargetKey;
private :
	UPROPERTY(EditAnywhere, Category = BehaviorTree, Meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, Category = BehaviorTree, Meta = (AllowPrivateAccess = true))
	class UBlackboardData* BBAsset;

};
