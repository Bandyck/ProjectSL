// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "Animation/AnimInstance.h"
#include "SLEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTSL_API USLEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public :
	USLEnemyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackMontage();
	void PlayHitReaction();
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	UFUNCTION()
	void AnimNotify_HitStart();
	UFUNCTION()
	void AnimNotify_HitEnd();
protected:
private:
public:
protected:
	uint8 AttackCount;
private :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitReactionMontage;
};
