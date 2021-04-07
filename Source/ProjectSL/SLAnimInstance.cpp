// Fill out your copyright notice in the Description page of Project Settings.


#include "SLAnimInstance.h"
#include "DrawDebugHelpers.h"

USLAnimInstance::USLAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	Skill_S_CoolTime = 3.0f;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animation/Character_Attack_Montage.Character_Attack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_MONTAGE(TEXT("/Game/Animation/Character_Dodge_Montage.Character_Dodge_Montage"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_Q_MONTAGE(TEXT("/Game/Animation/Character_Skill_Q_Montage.Character_Skill_Q_Montage"));
	if (SKILL_Q_MONTAGE.Succeeded())
	{
		Skill_Q_Montage = SKILL_Q_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_W_MONTAGE(TEXT("/Game/Animation/Character_Skill_W_Montage.Character_Skill_W_Montage"));
	if (SKILL_W_MONTAGE.Succeeded())
	{
		Skill_W_Montage = SKILL_W_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_R_MONTAGE(TEXT("/Game/Animation/Character_Skill_R_Montage.Character_Skill_R_Montage"));
	if (SKILL_R_MONTAGE.Succeeded())
	{
		Skill_R_Montage = SKILL_R_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_S_MONTAGE(TEXT("/Game/Animation/Character_Skill_S_Montage.Character_Skill_S_Montage"));
	if (SKILL_S_MONTAGE.Succeeded())
	{
		Skill_S_Montage = SKILL_S_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_F_MONTAGE(TEXT("/Game/Animation/Character_Skill_F_Montage.Character_Skill_F_Montage"));
	if (SKILL_F_MONTAGE.Succeeded())
	{
		Skill_F_Montage = SKILL_F_MONTAGE.Object;
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

void USLAnimInstance::PlayDodgeMontage()
{
	CHECK(!IsDead);
	Montage_Play(DodgeMontage, 1.15f);
}


void USLAnimInstance::PlaySkill_Q_Montage()
{
	CHECK(!IsDead);
	Montage_Play(Skill_Q_Montage, 1.0f);
}

void USLAnimInstance::PlaySkill_W_Montage()
{
	CHECK(!IsDead);
	//Skill_S_CoolTime = 3.0f;
	Montage_Play(Skill_W_Montage, 1.0f);
}

void USLAnimInstance::PlaySkill_R_Montage()
{
	CHECK(!IsDead);
	//Skill_S_CoolTime = 3.0f;
	Montage_Play(Skill_R_Montage, 1.0f);
}

void USLAnimInstance::PlaySkill_S_Montage()
{
	CHECK(!IsDead);
	//Skill_S_CoolTime = 3.0f;
	Montage_Play(Skill_S_Montage, 1.0f);
}

void USLAnimInstance::PlaySkill_F_Montage()
{
	CHECK(!IsDead);
	Montage_Play(Skill_F_Montage, 1.0f);
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
	if(Montage_IsPlaying(AttackMontage) == false)
	{
		LOG_S(Error);
		return;
	}
	OnNextAttackCheck.Broadcast();
}

void USLAnimInstance::AnimNotify_Skill_S_AttackCheck()
{
	OnSkill1HitCheck.Broadcast();
}


FName USLAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	CHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
