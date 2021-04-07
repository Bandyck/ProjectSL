// Fill out your copyright notice in the Description page of Project Settings.


#include "SLSpiderBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectSL.h"

const FName ASLSpiderBossAIController::TargetKey(TEXT("Target"));

ASLSpiderBossAIController::ASLSpiderBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/SpiderBoss/SpiderBoss_BlackBoard.SpiderBoss_BlackBoard"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/SpiderBoss/SpiderBoss_BehaviorTree.SpiderBoss_BehaviorTree"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}


void ASLSpiderBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			LOG(Error, TEXT("AIController couldn't run behavior tree!"));
			return;
		}
	}
}
