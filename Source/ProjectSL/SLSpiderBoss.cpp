// Fill out your copyright notice in the Description page of Project Settings.


#include "SLSpiderBoss.h"
#include "SLSpiderBossAnimInstance.h"

// Sets default values
ASLSpiderBoss::ASLSpiderBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	JumpAttackRange = 600.0f;
	
	/*static ConstructorHelpers::FObjectFinder<AActor> CIRCLE_DECAL(TEXT("/Game/Blueprints/SpiderBoss/CircleIndicatorDecal.CircleIndicatorDecal"));
	if (CIRCLE_DECAL.Succeeded())
	{
		CircleIndicator = CIRCLE_DECAL.Object;
	}*/

}

// Called when the game starts or when spawned
void ASLSpiderBoss::BeginPlay()
{
	Super::BeginPlay();
	//CircleIndicator = GetWorld()->SpawnActor(CircleIndicator->StaticClass());
	/*UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/SpiderBoss/CircleIndicatorDecal.CircleIndicatorDecal")));

	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!SpawnActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return;
	}

	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return;
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CircleIndicator = World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, SpawnParams);
	CircleIndicator->SetActorHiddenInGame(true);
	CircleIndicator->SetActorTickEnabled(false);*/

	CircleIndicator = MakeIndicator("/Game/Blueprints/SpiderBoss/CircleIndicatorDecal.CircleIndicatorDecal");
	CircleProgressIndicator = MakeIndicator("/Game/Blueprints/SpiderBoss/CircleProgressIndicatorDecal.CircleProgressIndicatorDecal");
}

// Called every frame
void ASLSpiderBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASLSpiderBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASLSpiderBoss::JumpAttack()
{
	USLSpiderBossAnimInstance* AnimInstance = Cast<USLSpiderBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		LOG_S(Error);
		return;
	}
	
	AnimInstance->JumpAttackStart();
}

AActor* ASLSpiderBoss::MakeIndicator(FString bluePrintPath)
{
	AActor* newIndicator = nullptr;
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, *bluePrintPath));

	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!SpawnActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return nullptr;
	}

	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return nullptr;
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	newIndicator = World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, SpawnParams);
	newIndicator->SetActorHiddenInGame(true);
	newIndicator->SetActorTickEnabled(false);
	return newIndicator;
}
