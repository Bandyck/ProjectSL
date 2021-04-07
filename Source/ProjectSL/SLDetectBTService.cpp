// Fill out your copyright notice in the Description page of Project Settings.


#include "SLDetectBTService.h"
#include "SLEnemyAIController.h"
#include "SLCharacter.h"
#include "SLEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

USLDetectBTService::USLDetectBTService()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
	DetectDistance = 600.f;
}

void USLDetectBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	CHECK(ControllingPawn != nullptr);
	
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();

	CHECK(World != nullptr);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(DetectDistance), CollisionQueryParams);

	if(bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ASLCharacter* Character = Cast<ASLCharacter>(OverlapResult.GetActor());
			if(Character && Character->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASLEnemyAIController::TargetKey, Character);
				/*ASLEnemy* Enemy = Cast<ASLEnemy>(ControllingPawn);
				if(Enemy == nullptr)
				{
					LOG_S(Error);
				}*/
				/*DrawDebugSphere(World, Center, DetectDistance, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Blue, false, 0.2f);*/
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ASLEnemyAIController::TargetKey, nullptr);
	
	DrawDebugSphere(World, Center, DetectDistance, 16, FColor::Red, false, 0.2f);
}