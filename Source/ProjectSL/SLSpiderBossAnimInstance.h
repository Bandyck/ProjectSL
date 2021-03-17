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
	void AnimNotify_JumpAttackEnd();
	UFUNCTION()
	void AnimNotify_DropRock();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* JumpAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
private:
};
