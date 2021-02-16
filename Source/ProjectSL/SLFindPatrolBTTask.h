// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SLFindPatrolBTTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLFindPatrolBTTask : public UBTTaskNode
{
	GENERATED_BODY()
public :
	USLFindPatrolBTTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
