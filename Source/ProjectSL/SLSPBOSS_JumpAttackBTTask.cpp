// Fill out your copyright notice in the Description page of Project Settings.


#include "SLSPBOSS_JumpAttackBTTask.h"
#include "SLSpiderBossAnimInstance.h"
#include "SLSpiderBossAIController.h"
#include "SLSpiderBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

USLSPBOSS_JumpAttackBTTask::USLSPBOSS_JumpAttackBTTask() : isAttacking(false)
{
	NodeName = TEXT("JumpAttack");
	bNotifyTick = true;
}

EBTNodeResult::Type USLSPBOSS_JumpAttackBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ASLSpiderBoss* Enemy = Cast<ASLSpiderBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
	{
		LOG_S(Error);
		return EBTNodeResult::Failed;
	}
	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASLSpiderBossAIController::TargetKey));
	if (Target == nullptr)
	{
		LOG_S(Error);
		return EBTNodeResult::Failed;
	}
	
	TargetPos = Target->GetActorLocation();
	TargetPos.Z -= Target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	StartPos = Enemy->GetActorLocation();
	JumpDir = Target->GetActorLocation() - Enemy->GetActorLocation();
	
	Enemy->GetCircleIndicator()->SetActorHiddenInGame(false);
	Enemy->GetCircleIndicator()->SetActorTickEnabled(true);
	Enemy->GetCircleIndicator()->SetActorLocation(TargetPos);

	Enemy->GetCircleProgressIndicator()->SetActorHiddenInGame(false);
	Enemy->GetCircleProgressIndicator()->SetActorTickEnabled(true);
	Enemy->GetCircleProgressIndicator()->SetActorLocation(TargetPos);
	ProgressIndicator = Enemy->GetCircleProgressIndicator();
	ProgressIndicator->SetActorScale3D(FVector(0.01f, 0.01f, 0.01f));
	
	LOG(Warning, TEXT("%s"), *StartPos.ToString());
	Enemy->JumpAttackEnd.AddLambda([this]()->void
		{
			isAttacking = false;
		});
	Enemy->JumpAttack();
	AnimLength = Cast<USLSpiderBossAnimInstance>(Enemy->GetMesh()->GetAnimInstance())->GetCurrentActiveMontage()->GetPlayLength();
	LOG(Warning, TEXT("%f"), AnimLength);
	isAttacking = true;
	return EBTNodeResult::InProgress;
}

void USLSPBOSS_JumpAttackBTTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASLSpiderBoss* Enemy = Cast<ASLSpiderBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
	{
		LOG_S(Error);
		return;
	}
	
	if(isAttacking == false)
	{
		Enemy->GetCircleIndicator()->SetActorHiddenInGame(true);
		Enemy->GetCircleIndicator()->SetActorTickEnabled(false);
		Enemy->GetCircleProgressIndicator()->SetActorHiddenInGame(true);
		Enemy->GetCircleProgressIndicator()->SetActorTickEnabled(false);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	/*FVector newLoc;
	newLoc.X = FMath::Lerp(0.f, JumpDir.X, DeltaSeconds / AnimLength);
	newLoc.Y = FMath::Lerp(0.f, JumpDir.Y, DeltaSeconds / AnimLength);
	newLoc.Z = 0;
	Enemy->SetActorLocation(Enemy->GetActorLocation() + newLoc);*/

	FVector newProgressScale = FMath::Lerp(FVector::ZeroVector, FVector::OneVector, DeltaSeconds / AnimLength);
	newProgressScale.X = 1.0f;
	ProgressIndicator->SetActorScale3D(ProgressIndicator->GetActorScale3D() + newProgressScale);

	FVector LookVector = TargetPos - StartPos;
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, GetWorld()->DeltaTimeSeconds, 2.0f));
}


