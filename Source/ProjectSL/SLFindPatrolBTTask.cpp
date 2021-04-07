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
	FVector NextPatrol;

	if(NavSystem->K2_GetRandomLocationInNavigableRadius(GetWorld(), Origin, NextPatrol, 500.f))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASLEnemyAIController::PatrolPosKey, NextPatrol);
		return EBTNodeResult::Succeeded;
	}
	LOG(Error, TEXT("%s"), *OwnerComp.GetOwner()->GetName());
	return EBTNodeResult::Failed;
}

