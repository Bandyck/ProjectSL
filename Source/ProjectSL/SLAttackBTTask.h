// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SLAttackBTTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLAttackBTTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public :
	USLAttackBTTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected :
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private :
	bool IsAttacking;
};
