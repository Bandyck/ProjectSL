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
	float AttackRange = 600.f;
	ASLEnemy* ControllingPawn = Cast<ASLEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if(ControllingPawn != nullptr)
	{
		AttackRange = ControllingPawn->GetAttackRange();
	}
	//CHECK(ControllingPawn != nullptr);
	ASLCharacter* Target = Cast<ASLCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASLEnemyAIController::TargetKey));
	//CHECK(Target != nullptr);

	//LOG(Warning, TEXT("%f"), Target->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()));
	
	bResult = (Target->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn()) <= AttackRange);
	return bResult;
}

