// Fill out your copyright notice in the Description page of Project Settings.


#include "SLEnemyWidget.h"
#include "SLEnemy.h"
#include "Components/ProgressBar.h"

void USLEnemyWidget::BindEnemy(ASLEnemy* Enemy)
{
	GetHPRatio.BindUFunction(Enemy, FName("GetHPRatio"));
	Enemy->OnHPChange.AddUObject(this, &USLEnemyWidget::UpdateHPBar);
}

void USLEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	CHECK(nullptr != HPProgressBar);
	UpdateHPBar();
}

void USLEnemyWidget::UpdateHPBar()
{
	if(GetHPRatio.IsBound() == true)
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(GetHPRatio.Execute());
		}
	}
}

