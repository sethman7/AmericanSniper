// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool/ASWeaponData.h"
#include "ASWeaponItem.generated.h"

UCLASS()
class ASPROTOTYPE_API AASWeaponItem : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UASWeaponData> Data;

};
