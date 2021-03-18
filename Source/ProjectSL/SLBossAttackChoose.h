// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SLBossAttackChoose.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLBossAttackChoose : public UBTTaskNode
{
	GENERATED_BODY()

public :
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
