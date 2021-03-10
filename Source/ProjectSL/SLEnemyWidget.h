// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLEnemyWidget.generated.h"

class ASLEnemy;


DECLARE_DELEGATE_RetVal(float, FHPRatioDelegate);

/**
 * 
 */
UCLASS()
class PROJECTSL_API USLEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	void BindEnemy(ASLEnemy* Enemy);
protected :
	void NativeConstruct() override;
private :
	void UpdateHPBar();
public :

protected :

private :
	UPROPERTY()
	class UProgressBar* HPProgressBar;
	FHPRatioDelegate GetHPRatio;
};
