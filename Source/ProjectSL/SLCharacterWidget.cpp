// Fill out your copyright notice in the Description page of Project Settings.


#include "SLCharacterWidget.h"
#include "SLCharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"

void USLCharacterWidget::BindCharacterStat(USLCharacterStatComponent* NewCharacterStat)
{
	CHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &USLCharacterWidget::UpdateHPWidget);
	NewCharacterStat->OnMPChanged.AddUObject(this, &USLCharacterWidget::UpdateMPWidget);
}

void USLCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	/*HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	CHECK(nullptr != HPProgressBar);*/
	UWHPWidget = Cast<UUserWidget>(GetWidgetFromName("HPWidget"));
	CHECK(nullptr != UWHPWidget);
	HPImage = Cast<UImage>(UWHPWidget->GetWidgetFromName("Image"));
	CHECK(nullptr != HPImage);

	UUserWidget* MPWidget = Cast<UUserWidget>(GetWidgetFromName("MPWidget"));
	MPImage = Cast<UImage>(MPWidget->GetWidgetFromName("Image"));
	UpdateHPWidget();
	UpdateMPWidget();
}

void USLCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		HPImage->GetDynamicMaterial()->SetScalarParameterValue(FName("percentage"), CurrentCharacterStat->GetHPRatio());
		/*if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}*/
	}
}

void USLCharacterWidget::UpdateMPWidget()
{
	if(CurrentCharacterStat.IsValid())
	{
		MPImage->GetDynamicMaterial()->SetScalarParameterValue(FName("percentage"), CurrentCharacterStat->GetMPRatio());
	}
}

