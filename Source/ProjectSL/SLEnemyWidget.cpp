// Fill out your copyright notice in the Description page of Project Settings.


#include "SLEnemyWidget.h"
#include "SLSpiderBoss.h"
#include "SLEnemy.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void USLEnemyWidget::BindEnemy(ASLEnemy* Enemy)
{
	GetHPRatio.BindUFunction(Enemy, FName("GetHPRatio"));
	Enemy->OnHPChange.AddUObject(this, &USLEnemyWidget::UpdateHPBar);
}

void USLEnemyWidget::BindBoss(ASLSpiderBoss* SpiderBoss)
{
	/*GetHPRatio.BindUObject(SpiderBoss, );
	SpiderBoss->*/
	GetHPRatio.BindUObject(SpiderBoss, &ASLSpiderBoss::GetHPRatio);
	SpiderBoss->HPChanged.AddUObject(this, &USLEnemyWidget::UpdateHPBar);
}


void USLEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPImage = Cast<UImage>(GetWidgetFromName(TEXT("HPBar")));
	CHECK(nullptr != HPImage);
	/*HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	CHECK(nullptr != HPProgressBar);
	UpdateHPBar();*/
}

void USLEnemyWidget::UpdateHPBar()
{
	if(GetHPRatio.IsBound() == true)
	{
		if (nullptr != HPImage)
		{
			HPImage->GetDynamicMaterial()->SetScalarParameterValue(FName("percentage"), GetHPRatio.Execute());
		}
	}
}

