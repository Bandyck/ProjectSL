// Fill out your copyright notice in the Description page of Project Settings.

#include "SLSpiderBossAnimInstance.h"
#include "SLSpiderBoss.h"
#include "SLCharacter.h"
#include "SLSPBOSS_JumpAttackBTTask.h"
#include "SLSpiderBossAIController.h"
#include "SLBossProjectile.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "DrawDebugHelpers.h"

USLSpiderBossAnimInstance::USLSpiderBossAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontage(TEXT("/Game/Blueprints/SpiderBoss/AnimMontage/Archanid_Jump_Montage.Archanid_Jump_Montage"));
	if (AnimMontage.Succeeded())
	{
		JumpAttackMontage = AnimMontage.Object;
	}
	CurrentPawnTurnSpeed = 0;
}


void USLSpiderBossAnimInstance::JumpAttackStart()
{
	StopAllMontages(0.2f);
	Montage_Play(JumpAttackMontage);
}

void USLSpiderBossAnimInstance::BaseAttackPlay()
{
	StopAllMontages(0.1f);
	Montage_Play(BaseAttack);
}

void USLSpiderBossAnimInstance::RangeAttackPlay()
{
	StopAllMontages(0.1f);
	Montage_Play(RangeAttack);
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

void USLSpiderBossAnimInstance::AnimNotify_BaseAttackRightCheck()
{
	float AttackRange = 400.f;
	ASLSpiderBoss* Enemy = Cast<ASLSpiderBoss>(TryGetPawnOwner());
	TArray<FHitResult> HitResults;
	
	FVector Center = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * AttackRange * 0.5f + Enemy->GetActorForwardVector() * 400.f - Enemy->GetActorRightVector() * 100.f;
	Center.Z -= Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FCollisionQueryParams Params(NAME_None, false, Enemy);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		Center - Enemy->GetActorForwardVector() * AttackRange * 0.5f,
		Center + Enemy->GetActorForwardVector() * AttackRange * 0.5f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRange * 0.5f),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = Enemy->GetActorForwardVector() * AttackRange;
	float HalfHeight = AttackRange * 0.5f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	float DebugLifeTime = 3.0f;

	DrawDebugSphere(
		GetWorld(),
		Center,
		HalfHeight,
		16,
		DrawColor,
	false,
		DebugLifeTime);
	
	
	/*DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRange,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);*/

#endif // ENABLE_DRAW_DEBUG

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			ASLCharacter* Character = Cast<ASLCharacter>(HitResult.Actor);
			if (Character != nullptr)
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(20.f, DamageEvent, Enemy->GetController(), Enemy);
				break;
			}
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetOwningActor()->GetWorld(), Enemy->GetLandDestroyParticle(), Center, FRotator::ZeroRotator);
	Enemy->CameraShake();
}

void USLSpiderBossAnimInstance::AnimNotify_BaseAttackLeftCheck()
{
	float AttackRange = 400.f;
	ASLSpiderBoss* Enemy = Cast<ASLSpiderBoss>(TryGetPawnOwner());
	TArray<FHitResult> HitResults;

	FVector Center = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * AttackRange * 0.5f + Enemy->GetActorForwardVector() * 400.f + Enemy->GetActorRightVector() * 100.f;
	Center.Z -= Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FCollisionQueryParams Params(NAME_None, false, Enemy);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		Center,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRange * 0.5f),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = Enemy->GetActorForwardVector() * AttackRange;
	float HalfHeight = AttackRange * 0.5f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	float DebugLifeTime = 3.0f;

	DrawDebugSphere(
		GetWorld(),
		Center,
		HalfHeight,
		16,
		DrawColor,
		false,
		DebugLifeTime);


	/*DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRange,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);*/

#endif // ENABLE_DRAW_DEBUG

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			ASLCharacter* Character = Cast<ASLCharacter>(HitResult.Actor);
			if (Character != nullptr)
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(20.f, DamageEvent, Enemy->GetController(), Enemy);
				break;
			}
		}
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetOwningActor()->GetWorld(), Enemy->GetLandDestroyParticle(), Center, FRotator::ZeroRotator);
	Enemy->CameraShake();
}

void USLSpiderBossAnimInstance::AnimNotify_BaseAttackEnd()
{
	ASLSpiderBoss* Enemy = Cast<ASLSpiderBoss>(TryGetPawnOwner());
	Enemy->BaseAttackEnd.Broadcast();
}

void USLSpiderBossAnimInstance::AnimNotify_ProjectileSpawn()
{
	LOG_S(Warning);

	FVector ProjectileLoc = GetOwningActor()->GetActorLocation();
	ProjectileLoc.Z -= Cast<ASLSpiderBoss>(TryGetPawnOwner())->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	ASLBossProjectile* BossProjectile = GetWorld()->SpawnActor<ASLBossProjectile>();
	BossProjectile->SetProjectileDir(GetOwningActor()->GetActorForwardVector());
	BossProjectile->SetActorLocation(ProjectileLoc);
}


void USLSpiderBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* Pawn = TryGetPawnOwner();

	if (!::IsValid(Pawn)) return;

	CurrentPawnSpeed = Pawn->GetVelocity().Size();
}

