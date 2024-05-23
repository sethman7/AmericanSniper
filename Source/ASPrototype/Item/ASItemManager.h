// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ASItemManager.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UASItemManager : public UObject
{
	GENERATED_BODY()

public:
	void PlayerGetItem(class AASItemBox *item,class AASCharacter* player);
	
};
