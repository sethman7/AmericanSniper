// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ASGameInstance.h"

UASGameInstance::UASGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/PlayerData.PlayerData");
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_ASCHARACTER(*CharacterDataPath);
	ASCharaterTable = DT_ASCHARACTER.Object;
}

void UASGameInstance::Init()
{
	Super::Init();
}
