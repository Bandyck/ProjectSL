// Fill out your copyright notice in the Description page of Project Settings.


#include "SLSkillSlot.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMaterialLibrary.h"

USLSkillSlot::USLSkillSlot(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
, CooldownMaterial(nullptr)
, CooldownMaterialInst(nullptr)
, IconTexture2D(nullptr)
, CoolTime(0)
, RemainCoolTime(0)
, OnCooldown(false)
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/UI_Kit/Materials/M_Cooldown.M_Cooldown'"));

	if (Material.Object != NULL)
	{
		CooldownMaterial = (UMaterial*)Material.Object;
	}
	else
	{
		LOG_S(Error);
	}
}


void USLSkillSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	Base_CooldownBorder = Cast<UBorder>(GetWidgetFromName("CooldownBorder"));
	Base_IconBorder = Cast<UBorder>(GetWidgetFromName("IconBorder"));
	if(Base_CooldownBorder == nullptr || Base_IconBorder == nullptr)
	{
		LOG_S(Error);
		return;
	}
}

void USLSkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OnCooldown)
	{
		RemainCoolTime -= InDeltaTime;
		CooldownMaterialInst->SetScalarParameterValue("percent", RemainCoolTime / CoolTime);
		if(RemainCoolTime < 0)
		{
			OnCooldown = false;
		}
	}
}


void USLSkillSlot::SetNewContent(UTexture2D* Texture2D, float& CoolTime_)
{
	CooldownMaterialInst = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), CooldownMaterial);
	if(CooldownMaterialInst == nullptr || Base_CooldownBorder == nullptr)
	{
		LOG_S(Error);
		return;
	}
	Base_CooldownBorder->SetBrushFromMaterial(CooldownMaterialInst);

	FSlateBrush IconBrush;
	IconBrush.ImageSize = FVector2D(32.f, 32.f);
	IconBrush.SetResourceObject(Texture2D);
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	Base_IconBorder->SetBrush(IconBrush);
	/*Base_IconBorder->SetBrushFromTexture(Texture2D);
	Base_IconBorder->*/
	CoolTime = CoolTime_;
}

void USLSkillSlot::SetSlotText(FString& text)
{
	UTextBlock* Text =  Cast<UTextBlock>(GetWidgetFromName("Text"));
	if(Text == nullptr)
	{
		LOG_S(Error);
		return;
	}
	Text->SetText(FText::FromString(text));
}


void USLSkillSlot::UseSlot()
{
	if(OnCooldown == true)
	{
		return;
	}
	OnCooldown = true;
	RemainCoolTime = CoolTime;
}
