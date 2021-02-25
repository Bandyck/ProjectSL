// Fill out your copyright notice in the Description page of Project Settings.


#include "SLTurnToTargetBTTask.h"
#include "SLEnemyAIController.h"
#include "SLCharacter.h"
#include "SLEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

USLTurnToTargetBTTask::USLTurnToTargetBTTask()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type USLTurnToTargetBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASLEnemy* Owner = Cast<ASLEnemy>(OwnerComp.GetAIOwner()->GetPawn());
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
	Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), TargetRot, GetWorld()->DeltaTimeSeconds, 2.0f));
	return EBTNodeResult::Succeeded;
}
