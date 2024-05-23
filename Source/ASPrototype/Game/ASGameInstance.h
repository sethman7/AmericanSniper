// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "ASGameInstance.generated.h"



USTRUCT(BlueprintType)
struct FASCharacterData:public FTableRowBase
{
	GENERATED_BODY()
public:
	FASCharacterData() : MaxHp(100.0f), MaxBulletNum(100.0f), MaxMagazineNum(10.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxBulletNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxMagazineNum;
};



UCLASS()
class ASPROTOTYPE_API UASGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UASGameInstance();

	virtual void Init() override;

private:
	UPROPERTY()
	class UDataTable* ASCharaterTable;
	
};
