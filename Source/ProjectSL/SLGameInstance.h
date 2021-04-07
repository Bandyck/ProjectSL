// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectSL.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "SLGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSLCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSLCharacterData() : Level(1), MaxHP(100.0f), MaxMP(100.0f), Attack(10.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxMP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;
};
/**
 * 
 */
UCLASS()
class PROJECTSL_API USLGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USLGameInstance();

	virtual void Init() override;
	FSLCharacterData* GetSLCharacterData(int32 Level);

private:
	UPROPERTY()
		class UDataTable* SLCharacterTable;
};
