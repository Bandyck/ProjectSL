// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "Blueprint/UserWidget.h"
#include "SLSkillSlot.generated.h"

/**
 * 
 */

class UBorder;

UCLASS()
class PROJECTSL_API USLSkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	USLSkillSlot(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
	void SetNewContent(UTexture2D* Texture2D, float& CoolTime_);
	UFUNCTION()
	void SetSlotText(FString& text);
	UFUNCTION()
	void UseSlot();
	UFUNCTION()
	bool IsOnCooldown() const { return OnCooldown; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UBorder* Base_CooldownBorder;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	UBorder* Base_IconBorder;
public:

protected:
	UMaterial* CooldownMaterial;
	UMaterialInstanceDynamic* CooldownMaterialInst;
	UTexture2D* IconTexture2D;
	float CoolTime;
	float RemainCoolTime;
	bool OnCooldown;
};
