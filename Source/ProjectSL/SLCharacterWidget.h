// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "Blueprint/UserWidget.h"
#include "SLCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class USLCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class USLCharacterStatComponent> CurrentCharacterStat;
	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
