// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ASMainGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UASMainGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//void BindCharacterStat(class UASCharacterStatComponent* CharacterStat);
	//void BindPlayerState(class AASPlayerState);
	void BindPlayerBase(class AASCharacterBase* PlayerBase);
	void BindPlayerBaseForBullet(class AASCharacterBase* PlayerBase);
	void BindPlayerBaseForMagazine(class AASCharacterBase* PlayerBase);
	void BindPlayerBaseForItem(class AASCharacterBase* PlayerBase);
	void BindPlayerBaseForMagnification(class AASCharacterBase* PlayerBase);
	//void ConnectUIwithData();

protected:
	virtual void NativeConstruct() override;
	//void UpdateCharacterStat();
	void UpdatePlayerState();
	void UpdateBulletUI();
	void UpdateMagazineUI();
	void UpdateItemUI();
	void UpdateMagnificationUI();

private:
	//TWeakObjectPtr<class UASCharacterStatComponent> CurrentCharacterStat;
	//TWeakObjectPtr<class AASPlayerState> CurrentPlayerState;
	TWeakObjectPtr<class AASCharacterBase> CurrentPlayerBase;

	UPROPERTY()
	class UProgressBar* HpBar;

	UPROPERTY()
	class UEditableTextBox* BulletNumUI;

	UPROPERTY()
	class UEditableTextBox* MagazineNum;

	UPROPERTY()
	class UTextBlock* ItemNum;

	UPROPERTY()
	class UTextBlock* MagnificationNum;

	UPROPERTY()
	class UProgressBar* MagnificationBar;
};
