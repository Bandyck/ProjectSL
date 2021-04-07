// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "Animation/AnimInstance.h"
#include "SLSpiderBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLSpiderBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public :
	USLSpiderBossAnimInstance();
	void JumpAttackStart();
	UFUNCTION()
	void BaseAttackPlay();
	UFUNCTION()
	void RangeAttackPlay();
	UFUNCTION()
	void BreathAttackPlay();
	UFUNCTION()
	void DeadMontagePlay();
	UFUNCTION()
	void AnimNotify_JumpAttackEnd();
	UFUNCTION()
	void AnimNotify_DropRock();
	UFUNCTION()
	void AnimNotify_BaseAttackRightCheck();
	UFUNCTION()
	void AnimNotify_BaseAttackLeftCheck();
	UFUNCTION()
	void AnimNotify_BaseAttackEnd();
	UFUNCTION()
	void AnimNotify_BreathStart();
	UFUNCTION()
	void AnimNotify_BreathEnd();
	UFUNCTION()
	void AnimNotify_ProjectileSpawn();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetCurrentPawnTurnSpeed(float Speed) { CurrentPawnTurnSpeed = Speed; }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* JumpAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BaseAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* RangeAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BreathAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnTurnSpeed;
	
	class ASLBossAOEAttack* InstancedAOEAttack;
private:
};
