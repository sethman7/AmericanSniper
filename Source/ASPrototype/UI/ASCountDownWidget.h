// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASCountDownWidget.generated.h"
/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UASCountDownWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY()
	class UEditableText* Timer;
	float CountDown;
	float Minus;
	bool start;
	class AASGameMode* curGameMode;
};
