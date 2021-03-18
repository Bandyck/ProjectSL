// Fill out your copyright notice in the Description page of Project Settings.


#include "SLTurnToTargetBTTask.h"
#include "SLEnemyAIController.h"
#include "SLCharacter.h"
#include "SLEnemy.h"
#include "SLSpiderBoss.h"
#include "SLSpiderBossAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

USLTurnToTargetBTTask::USLTurnToTargetBTTask()
{
	NodeName = TEXT("Turn");
	bNotifyTick = true;
}

EBTNodeResult::Type USLTurnToTargetBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if(Owner == nullptr)
	{
		LOG_S(Error);
		return EBTNodeResult::Failed;
	}

	ASLCharacter* Target = Cast<ASLCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASLEnemyAIController::TargetKey));
	if(Target == nullptr)
	{
		LOG_S(Error);
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - Owner->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	FRotator LookAtRot = FMath::RInterpTo(Owner->GetActorRotation(), TargetRot, GetWorld()->DeltaTimeSeconds, 2.0f);

	ASLSpiderBoss* SpiderBoss = Cast<ASLSpiderBoss>(Owner);
	if(SpiderBoss != nullptr)
	{
		USLSpiderBossAnimInstance* SpiderBossAnimInstance = Cast<USLSpiderBossAnimInstance>(SpiderBoss->GetMesh()->GetAnimInstance());
		SpiderBossAnimInstance->SetCurrentPawnTurnSpeed(Owner->GetActorRotation().Yaw - LookAtRot.Yaw);
		return EBTNodeResult::InProgress;
	}
	else
	{
		Owner->SetActorRotation(LookAtRot);
	}

	return EBTNodeResult::Succeeded;
}

void USLTurnToTargetBTTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if(Owner == nullptr)
	{
		LOG_S(Error);
	}

	ASLCharacter* Target = Cast<ASLCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASLEnemyAIController::TargetKey));
	if(Target == nullptr)
	{
		LOG_S(Error);
	}

	FVector LookVector = Target->GetActorLocation() - Owner->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	if(abs(Owner->GetActorRotation().Yaw - TargetRot.Yaw) < 10)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	FRotator LookAtRot = FMath::RInterpTo(Owner->GetActorRotation(), TargetRot, GetWorld()->DeltaTimeSeconds, 2.0f);

	ASLSpiderBoss* SpiderBoss = Cast<ASLSpiderBoss>(Owner);
	if(SpiderBoss != nullptr)
	{
		USLSpiderBossAnimInstance* SpiderBossAnimInstance = Cast<USLSpiderBossAnimInstance>(SpiderBoss->GetMesh()->GetAnimInstance());
		SpiderBossAnimInstance->SetCurrentPawnTurnSpeed(Owner->GetActorRotation().Yaw - LookAtRot.Yaw);
		LOG(Warning, TEXT("%f"), Owner->GetActorRotation().Yaw - LookAtRot.Yaw);
	}

	Owner->SetActorRotation(LookAtRot);
}
