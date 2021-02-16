// Fill out your copyright notice in the Description page of Project Settings.


#include "SLEnemyAnimInstance.h"

USLEnemyAnimInstance::USLEnemyAnimInstance()
{
	CurrentPawnSpeed = 0;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Blueprints/GreaterSpider_Attack_Montage.GreaterSpider_Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		//LOG_S(Warning);
		AttackMontage = ATTACK_MONTAGE.Object;
	}
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
}

