// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossAttackChoose.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USLBossAttackChoose::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("AttackType"), FMath::RandRange(0, 2));
	return EBTNodeResult::Failed;
}
