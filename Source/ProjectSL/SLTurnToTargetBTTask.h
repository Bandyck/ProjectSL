// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SLTurnToTargetBTTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLTurnToTargetBTTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public :
	USLTurnToTargetBTTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
