// Fill out your copyright notice in the Description page of Project Settings.

#include "SLSpiderBoss.h"
#include "SLSpiderBossAnimInstance.h"

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
}
