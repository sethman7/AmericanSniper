// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DetectItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UDetectItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UEditableTextBox* PressButtonText;

	UPROPERTY()
	class UTextBlock* ItemName;

	class AASItemBox* CurItem;

public:
	void UpdateItemName(FString newName);
	
};
