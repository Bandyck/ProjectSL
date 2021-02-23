// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SLSPBOSS_JumpAttackBTTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLSPBOSS_JumpAttackBTTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USLSPBOSS_JumpAttackBTTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public :
protected :
	bool isAttacking;
	FVector TargetPos;
	FVector StartPos;
	FVector JumpDir;
	float AnimLength;
	AActor* ProgressIndicator;
};
