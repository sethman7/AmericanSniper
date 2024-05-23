// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "AI/ASAIController.h"
#include "ASDetectWidget.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FUASDetectWidgetOnChangedSignature, bool b)
/**
 * 
 */


UCLASS()
class ASPROTOTYPE_API UASDetectWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	FWidgetAnimationDynamicEvent EndDelegate;


	void Onvisible();

	UFUNCTION()
	void OffVisible();

	void SetRedColor();
	void SetPercent(float f);
	void BlinkBar();
	UFUNCTION()
	/*void RunTimeline(float Value);*/
	//void StartChasing(bool b);

	void SetAngle(float angle);

	FORCEINLINE AActor* GetOwner() { return Owner; }
	FORCEINLINE void SetOwner(AActor* newOwner) { Owner = newOwner; }

	float CurPercent;
	float MaxPercent;
	bool DetectOn;

	UPROPERTY(EditAnywhere ,BlueprintReadWrite ,meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Blink;

protected:
	AActor* Owner;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UPROPERTY()
	TObjectPtr<class UProgressBar> DetectBar;


private:
	class AASAIController* AiRef;

};
