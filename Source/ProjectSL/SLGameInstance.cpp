// Fill out your copyright notice in the Description page of Project Settings.


#include "SLGameInstance.h"

USLGameInstance::USLGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Data/CharacterData.CharacterData");

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SLCharacter(*CharacterDataPath);
	CHECK(DT_SLCharacter.Succeeded());
	SLCharacterTable = DT_SLCharacter.Object;
	CHECK(SLCharacterTable->GetRowMap().Num() > 0);
}

void USLGameInstance::Init()
{
	Super::Init();
}

FSLCharacterData* USLGameInstance::GetSLCharacterData(int32 Level)
{
	return SLCharacterTable->FindRow<FSLCharacterData>(*FString::FromInt(Level), TEXT(""));
}
