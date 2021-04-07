// Fill out your copyright notice in the Description page of Project Settings.


#include "SLEnemyAnimInstance.h"

#include "BrainComponent.h"
#include "SLEnemy.h"
#include "SLEnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "SLCharacter.h"

USLEnemyAnimInstance::USLEnemyAnimInstance()
{
	CurrentPawnSpeed = 0;
	AttackCount = 0;

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Blueprints/GreaterSpider_Attack_Montage.GreaterSpider_Attack_Montage"));
	//if (ATTACK_MONTAGE.Succeeded())
	//{
	//	//LOG_S(Warning);
	//	AttackMontage = ATTACK_MONTAGE.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> HITREACTION_MONTAGE(TEXT("/Game/Blueprints/GreaterSpider_ReactionFront_Montage.GreaterSpider_ReactionFront_Montage"));
	//if (ATTACK_MONTAGE.Succeeded())
	//{
	//	//LOG_S(Warning);
	//	HitReactionMontage = HITREACTION_MONTAGE.Object;
	//}
}

void USLEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* Pawn = TryGetPawnOwner();

	if (!::IsValid(Pawn)) return;

	CurrentPawnSpeed = Pawn->GetVelocity().Size();
}

void USLEnemyAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
	else 
	{
		LOG_S(Error);
	}
}

void USLEnemyAnimInstance::PlayHitReaction()
{
	if (Montage_IsPlaying(AttackMontage))
	{
		Montage_Stop(0.3f, AttackMontage);
	}
	Montage_Play(HitReactionMontage, 1.0f);
	ASLEnemy* Enemy = Cast<ASLEnemy>(TryGetPawnOwner());
	if (Enemy == nullptr)
	{
		LOG_S(Error);
	}
	Enemy->OnAttackEnd.Broadcast();
	ASLEnemyAIController* AIController = Cast<class ASLEnemyAIController>(Enemy->GetController());
	if (AIController != nullptr)
	{
		AIController->GetBrainComponent()->StopLogic("HitStart");
	}
	else
	{
		LOG_S(Error);
	}
}

void USLEnemyAnimInstance::PlayDeadMontage()
{
	StopAllMontages(0.1f);
	StopSlotAnimation();
	Montage_Play(DeadMontage, 1.0f);
	FTimerHandle WaitHandle;
	float WaitTime = DeadMontage->GetPlayLength() * 0.95f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			Montage_Pause();
		}), WaitTime, false);
}

float USLEnemyAnimInstance::GetDeadMontageTime()
{
	return DeadMontage->GetPlayLength();
}

void USLEnemyAnimInstance::AnimNotify_AttackHitCheck()
{
	LOG(Warning, TEXT("%s"), *AttackMontage->GetName());
	ASLEnemy* Enemy = Cast<ASLEnemy>(TryGetPawnOwner());
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, Enemy);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		Enemy->GetActorLocation(),
		Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * Enemy->GetAttackRange(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Enemy->GetAttackRange()),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = Enemy->GetActorForwardVector() * Enemy->GetAttackRange();
	FVector Center = Enemy->GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = Enemy->GetAttackRange() * 0.5f + Enemy->GetAttackRadius();
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	float DebugLifeTime = 3.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		Enemy->GetAttackRadius(),
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif // ENABLE_DRAW_DEBUG

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			ASLCharacter* Character = Cast<ASLCharacter>(HitResult.Actor);
			if (Character != nullptr)
			{
				FDamageEvent DamageEvent;
				HitResult.Actor->TakeDamage(Enemy->GetAttackPower(), DamageEvent, Enemy->GetController(), Enemy);
				AttackCount++;
				if (AttackCount == Enemy->GetAttackComboCount())
				{
					AttackCount = 0;
					Montage_Stop(1.0f, AttackMontage);
					Enemy->OnAttackEnd.Broadcast();
				}
				return;
			}
		}
	}
	AttackCount = 0;
	Montage_Stop(1.0f, AttackMontage);
	Enemy->OnAttackEnd.Broadcast();

}

//void USLEnemyAnimInstance::AnimNotify_HitStart()
//{
//	ASLEnemy* Enemy = Cast<ASLEnemy>(TryGetPawnOwner());
//	if(Enemy == nullptr)
//	{
//		LOG_S(Error);
//	}
//	Enemy->OnAttackEnd.Broadcast();
//	ASLEnemyAIController* AIController = Cast<class ASLEnemyAIController>(Enemy->GetController());
//	if (AIController != nullptr)
//	{
//		AIController->GetBrainComponent()->StopLogic("HitStart");
//	}
//	else
//	{
//		LOG_S(Error);
//	}
//}

void USLEnemyAnimInstance::AnimNotify_HitEnd()
{
	ASLEnemy* Enemy = Cast<ASLEnemy>(TryGetPawnOwner());
	ASLEnemyAIController* AIController = Cast<class ASLEnemyAIController>(Enemy->GetController());
	if (AIController != nullptr)
	{
		AIController->GetBrainComponent()->StartLogic();
	}
	else
	{
		LOG_S(Error);
	}
}



