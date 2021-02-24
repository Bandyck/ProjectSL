// Fill out your copyright notice in the Description page of Project Settings.

#include "SLSpiderBoss.h"
#include "SLSpiderBossAnimInstance.h"
#include "SLSPBOSS_JumpAttackBTTask.h"
#include "SLSpiderBossAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

USLSpiderBossAnimInstance::USLSpiderBossAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontage(TEXT("/Game/Blueprints/SpiderBoss/AnimMontage/JumpAttack_Montage.JumpAttack_Montage"));
	if (AnimMontage.Succeeded())
	{
		JumpAttackMontage = AnimMontage.Object;
	}

}


void USLSpiderBossAnimInstance::JumpAttackStart()
{
	StopAllMontages(0.2f);
	Montage_Play(JumpAttackMontage);
}


void USLSpiderBossAnimInstance::AnimNotify_JumpAttackEnd()
{
	ASLSpiderBoss* Enemy = Cast<ASLSpiderBoss>(TryGetPawnOwner());
	Enemy->JumpAttackEnd.Broadcast();
	UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(TryGetPawnOwner()->GetController()->GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
	if (BehaviorTree == nullptr)
	{
		LOG_S(Error);
		return;
	}
	const USLSPBOSS_JumpAttackBTTask* JumpAttackTask = Cast<USLSPBOSS_JumpAttackBTTask>(BehaviorTree->GetActiveNode());
	if (JumpAttackTask == nullptr)
	{
		LOG_S(Error);
		return;
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetOwningActor()->GetWorld(), Enemy->GetLandDestroyParticle(), JumpAttackTask->GetTargetPos(), FRotator::ZeroRotator);
}

void USLSpiderBossAnimInstance::AnimNotify_DropRock()
{
	UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(TryGetPawnOwner()->GetController()->GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
	if (BehaviorTree == nullptr)
	{
		LOG_S(Error);
		return;
	}
	const USLSPBOSS_JumpAttackBTTask* JumpAttackTask = Cast<USLSPBOSS_JumpAttackBTTask>(BehaviorTree->GetActiveNode());
	if (JumpAttackTask == nullptr)
	{
		LOG_S(Error);
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetOwningActor()->GetWorld(), Cast<ASLSpiderBoss>(TryGetPawnOwner())->GetRockDropParticle(), JumpAttackTask->GetTargetPos(), FRotator::ZeroRotator);
}
