// Fill out your copyright notice in the Description page of Project Settings.


#include "SLCharacterStatComponent.h"
#include "SLGameInstance.h"

// Sets default values for this component's properties
USLCharacterStatComponent::USLCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
	Level = 1;
}


// Called when the game starts
void USLCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	CalculatingRegeneration();
}

void USLCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	static float DeltaTimeSum = 0;
	DeltaTimeSum += DeltaTime;
	if(DeltaTimeSum > 0.1f)
	{
		DeltaTimeSum -= 0.1f;

		if(CurrentHP < CurrentStatData->MaxHP)
		{
			CurrentHP += Regeneration;
			if(CurrentHP > CurrentStatData->MaxHP)
			{
				CurrentHP = CurrentStatData->MaxHP;
			}
			OnHPChanged.Broadcast();
		}
		if(CurrentMP < CurrentStatData->MaxMP)
		{
			CurrentMP += Regeneration * 2;
			if(CurrentMP > CurrentStatData->MaxMP)
			{
				CurrentMP = CurrentStatData->MaxMP;
			}
			OnMPChanged.Broadcast();
		}
	}
}


void USLCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void USLCharacterStatComponent::CalculatingRegeneration()
{
	Regeneration = CurrentStatData->MaxHP * 0.001f;
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
		SetMP(CurrentStatData->MaxMP);

		CalculatingRegeneration();
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

void USLCharacterStatComponent::ConsumeMP(float MPAmount)
{
	CHECK(nullptr != CurrentStatData);
	SetMP(FMath::Clamp<float>(CurrentMP - MPAmount, 0.0f, CurrentStatData->MaxMP));
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

void USLCharacterStatComponent::SetMP(float NewMP)
{
	CurrentMP = NewMP;
	OnMPChanged.Broadcast();
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

float USLCharacterStatComponent::GetMPRatio()
{
	CHECK(nullptr != CurrentStatData, 0.0f);
	return (CurrentStatData->MaxMP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentMP / CurrentStatData->MaxMP);
}
