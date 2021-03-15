// Fill out your copyright notice in the Description page of Project Settings.


#include "SLPlayerController.h"
#include "SLCharacter.h"

ASLPlayerController::ASLPlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	//MyCharacter = GetPawn();
}

void ASLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Move", IE_Pressed, this, &ASLPlayerController::InputClickPressed);
	InputComponent->BindAction("Move", IE_Released, this, &ASLPlayerController::InputClickReleased);
	//InputComponent->BindAction("Attack", IE_Pressed, this, &ASLPlayerController::Attack);
}

void ASLPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bClickedMouse)
	{
		MoveToMouseCursor();
	}
}

void ASLPlayerController::InputClickPressed()
{
	bClickedMouse = true;
}

void ASLPlayerController::InputClickReleased()
{
	bClickedMouse = false;
}

void ASLPlayerController::SetNewDestination(const FVector DestLocation)
{
	//APawn* const MyCharacter = GetPawn();
	//CHECK(nullptr != MyCharacter);
	//if (MyCharacter)
	if(AcknowledgedPawn)
	{
		//float const Distance = FVector::Dist(DestLocation, MyCharacter->GetActorLocation());
		float const Distance = FVector::Dist(DestLocation, AcknowledgedPawn->GetActorLocation());
		ASLCharacter* ACharacter = Cast<ASLCharacter>(AcknowledgedPawn);
		if(ACharacter == nullptr)
		{
			LOG_S(Error);
			return;
		}
		if (Distance > 120.0f && ACharacter->IsMovable())
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ASLPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewDestination(Hit.ImpactPoint);
	}
}

//void ASLPlayerController::Attack()
//{
//
//}
