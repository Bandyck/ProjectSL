// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAnimInstance.h"

USLAnimInstance::USLAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	Skill_S_CoolTime = 3.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Blueprints/Character_Attack_Montage.Character_Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_S_MONTAGE(TEXT("/Game/Blueprints/Character_Skill_S_Montage.Character_Skill_S_Montage"));
	if (SKILL_S_MONTAGE.Succeeded())
	{
		Skill_S_Montage = SKILL_S_MONTAGE.Object;
	}
}

void USLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if(!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}

void USLAnimInstance::PlayAttackMontage()
{
	CHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void USLAnimInstance::PlaySkill_S_Montage()
{
	CHECK(!IsDead);
	Skill_S_CoolTime = 3.0f;
	Montage_Play(Skill_S_Montage, 1.0f);
}

void USLAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	CHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void USLAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void USLAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName USLAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	CHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
