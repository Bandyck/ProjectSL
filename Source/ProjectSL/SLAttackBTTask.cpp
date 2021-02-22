// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAttackBTTask.h"
#include "SLEnemy.h"
#include "SLEnemyAIController.h"

USLAttackBTTask::USLAttackBTTask()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type USLAttackBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASLEnemy* Enemy = Cast<ASLEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
	{
		LOG_S(Error);
		return EBTNodeResult::Failed;
	}

	Enemy->Attack();
	IsAttacking = true;
	Enemy->OnAttackEnd.AddLambda([this]()->void
		{
			IsAttacking = false;
		});
	return EBTNodeResult::InProgress;
}



void USLAttackBTTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
