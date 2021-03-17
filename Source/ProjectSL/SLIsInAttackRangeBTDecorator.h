// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "BehaviorTree/BTDecorator.h"
#include "SLIsInAttackRangeBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLIsInAttackRangeBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	USLIsInAttackRangeBTDecorator();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
public :

protected:
};
