// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SLDetectBTService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLDetectBTService : public UBTService
{
	GENERATED_BODY()
	
public :
	USLDetectBTService();

protected :
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private :
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	float DetectDistance;
};
