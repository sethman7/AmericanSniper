// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ASItemManager.h"
#include "ASItemBox.h"
#include "Character/ASCharacterPlayer.h"


void UASItemManager::PlayerGetItem(AASItemBox* item, AASCharacter* player)
{
	FString itemName = item->GetItemName();
	if (itemName == TEXT("Bullet"))
	{

	}
	else if (itemName == TEXT("Bandage"))
	{

	}
}
