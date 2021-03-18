// Fill out your copyright notice in the Description page of Project Settings.


#include "SLBossRandomAttackBTService.h"
#include "BehaviorTree/BlackboardComponent.h"

void USLBossRandomAttackBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("AttackType"), FMath::RandRange(0, 1));
}
