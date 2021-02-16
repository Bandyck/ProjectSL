// Fill out your copyright notice in the Description page of Project Settings.


#include "SLEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectSL.h"

const FName ASLEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName ASLEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ASLEnemyAIController::TargetKey(TEXT("Target"));

ASLEnemyAIController::ASLEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Blueprints/GreaterSpiderBlackBoard.GreaterSpiderBlackBoard"));
	if(BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Blueprints/GreaterSpiderBehaviorTree.GreaterSpiderBehaviorTree"));
	if(BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ASLEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if(!RunBehaviorTree(BTAsset))
		{
			LOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ASLEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ASLEnemyAIController::OnRepeatTimer()
{
}


