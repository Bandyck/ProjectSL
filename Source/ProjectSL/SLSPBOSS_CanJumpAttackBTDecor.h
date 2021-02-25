// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SLSPBOSS_CanJumpAttackBTDecor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLSPBOSS_CanJumpAttackBTDecor : public UBTDecorator
{
	GENERATED_BODY()

public:
	USLSPBOSS_CanJumpAttackBTDecor();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};