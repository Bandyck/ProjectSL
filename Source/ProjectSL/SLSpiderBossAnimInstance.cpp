// Fill out your copyright notice in the Description page of Project Settings.

#include "SLSpiderBossAnimInstance.h"
#include "SLSpiderBoss.h"
#include "SLSPBOSS_JumpAttackBTTask.h"
#include "SLSpiderBossAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "DrawDebugHelpers.h"

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

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, Enemy);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		JumpAttackTask->GetTargetPos()- 100.f,
		JumpAttackTask->GetTargetPos() + 100.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(200.f),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector Center = JumpAttackTask->GetTargetPos();
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	float DebugLifeTime = 3.0f;

	DrawDebugSphere(GetWorld(), Center, 200.f, 20, DrawColor, false, DebugLifeTime);
	DrawDebugSphere(GetWorld(), HitResult.Location, 10.f, 20, DrawColor, false, DebugLifeTime);

#endif // ENABLE_DRAW_DEBUG
	/*if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(10.0f, DamageEvent, Enemy->GetController(), Enemy);
			AttackCount++;
			if (AttackCount == Enemy->GetAttackComboCount())
			{
				Enemy->OnAttackEnd.Broadcast();
			}
		}
		else
		{
			AttackCount = 0;
			Montage_Stop(1.0f, AttackMontage);
			Enemy->OnAttackEnd.Broadcast();
		}
	}*/
	
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
