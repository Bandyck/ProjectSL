// Fill out your copyright notice in the Description page of Project Settings.


#include "SLFindPatrolBTTask.h"
#include "SLEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "ProjectSL.h"

USLFindPatrolBTTask::USLFindPatrolBTTask()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type USLFindPatrolBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	LOG_S(Warning);
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		LOG_S(Error);
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		LOG_S(Error);
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ASLEnemyAIController::HomePosKey);
	FNavLocation NextPatrol;

	if(NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASLEnemyAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}
	LOG_S(Error);
	return EBTNodeResult::Failed;
}

