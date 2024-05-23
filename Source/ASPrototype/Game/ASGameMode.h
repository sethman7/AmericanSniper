// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AS, Log, All);

/**
 * 
*/
UCLASS()
class ASPROTOTYPE_API AASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AASGameMode();
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	virtual void PostLogin(APlayerController* NewPlayer) override;

<<<<<<< HEAD
protected:
	virtual void BeginPlay() override;

=======
	bool IsEscape;

	class AASCharacterPlayer* Player;
	class AASFinalTarget* Target;
	void EscapeTimerStart(); // Å¸°Ù »ç¸Á½Ã ¹Ù·Î È£Ãâ
	void View_WinWidget();
	void View_LoseWidget();
	void GameOver();
	class TSubclassOf<UUserWidget> CountDownWidgetClass;

	class UASCountDownWidget* CountDownWidget;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
>>>>>>> aa978d577c1080692cf93d18e90275be5bbfa0de
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
<<<<<<< HEAD
=======

>>>>>>> aa978d577c1080692cf93d18e90275be5bbfa0de
};
