// Fill out your copyright notice in the Description page of Project Settings.


#include "SLCharacterStatComponent.h"
#include "SLGameInstance.h"

// Sets default values for this component's properties
USLCharacterStatComponent::USLCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
	Level = 1;
}


// Called when the game starts
void USLCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USLCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void USLCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto SLGameInstance = Cast<USLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	CHECK(nullptr != SLGameInstance);
	CurrentStatData = SLGameInstance->GetSLCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		LOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void USLCharacterStatComponent::SetDamage(float NewDamage)
{
	CHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void USLCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	LOG(Warning, TEXT("%f"), NewHP);
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float USLCharacterStatComponent::GetAttack()
{
	CHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float USLCharacterStatComponent::GetHPRatio()
{
	CHECK(nullptr != CurrentStatData, 0.0f);
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}
