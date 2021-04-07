// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAttackBTTask.h"
#include "SLEnemy.h"
#include "SLSpiderBoss.h"
#include "SLSpiderBossAnimInstance.h"
#include "SLEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	if (Enemy != nullptr)
	{
		Enemy->Attack();
		Enemy->OnAttackEnd.AddLambda([this]()->void
			{
				IsAttacking = false;
			});
		IsAttacking = true;
		return EBTNodeResult::InProgress;
	}

	ASLSpiderBoss* SpiderBoss = Cast<ASLSpiderBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(SpiderBoss != nullptr)
	{
		USLSpiderBossAnimInstance* SpiderBossAnimInstance = Cast<USLSpiderBossAnimInstance>(SpiderBoss->GetMesh()->GetAnimInstance());
		SpiderBossAnimInstance->SetCurrentPawnTurnSpeed(0);
		int32 attackType = OwnerComp.GetBlackboardComponent()->GetValueAsInt("AttackType");
		
		switch (attackType)
		{
			case 0 :
				{
					SpiderBoss->BaseAttack();
				}
			break;
			case 1 :
				{
					SpiderBoss->RangeAttack();
				}
			break;
			case 2 :
				{
					SpiderBoss->BreathAttack();
				}
			break;
		}
		SpiderBoss->BaseAttackEnd.AddLambda([this]()->void
			{
				IsAttacking = false;
			});
		IsAttacking = true;
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}



void USLAttackBTTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
