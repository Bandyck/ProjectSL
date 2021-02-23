// Fill out your copyright notice in the Description page of Project Settings.


#include "SLSPBOSS_CanJumpAttackBTDecor.h"

#include "ProjectSL.h"
#include "SLSpiderBoss.h"
#include "SLSpiderBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USLSPBOSS_CanJumpAttackBTDecor::USLSPBOSS_CanJumpAttackBTDecor()
{
	NodeName = TEXT("CanJumpAttack");
}

bool USLSPBOSS_CanJumpAttackBTDecor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ASLSpiderBoss* ControllingPawn = Cast<ASLSpiderBoss>(OwnerComp.GetAIOwner()->GetPawn());
	//CHECK(ControllingPawn != nullptr);

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASLSpiderBossAIController::TargetKey));
	if(Target == nullptr)
	{
		return false;
	}
	//CHECK(Target != nullptr);

	bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetJumpAttackRange());
	return bResult;
}