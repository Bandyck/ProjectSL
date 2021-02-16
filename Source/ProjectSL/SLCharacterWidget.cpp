// Fill out your copyright notice in the Description page of Project Settings.


#include "SLCharacterWidget.h"
#include "SLCharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/UniformGridPanel.h"

void USLCharacterWidget::BindCharacterStat(USLCharacterStatComponent* NewCharacterStat)
{
	CHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &USLCharacterWidget::UpdateHPWidget);
}

void USLCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	CHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void USLCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
