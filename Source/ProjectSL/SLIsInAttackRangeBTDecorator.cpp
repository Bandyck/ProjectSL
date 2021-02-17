// Fill out your copyright notice in the Description page of Project Settings.


#include "SLIsInAttackRangeBTDecorator.h"
#include "SLEnemyAIController.h"
#include "SLCharacter.h"
#include "SLEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

USLIsInAttackRangeBTDecorator::USLIsInAttackRangeBTDecorator()
{
	NodeName = TEXT("CanAttack");
}

bool USLIsInAttackRangeBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ASLEnemy* ControllingPawn = Cast<ASLEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	//CHECK(ControllingPawn != nullptr);

	ASLCharacter* Target = Cast<ASLCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASLEnemyAIController::TargetKey));
	//CHECK(Target != nullptr);

	bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetAttackRange());
	return bResult;
}
