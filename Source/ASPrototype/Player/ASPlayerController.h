// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/ASGameMode.h"
#include "GameFramework/PlayerController.h"
#include "ASPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API AASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 
	AASPlayerController();

	virtual void PostInitializeComponents() override;

	virtual void OnPossess(APawn* aPawn) override;

	class UASMainGameWidget* GetHUDWidget();

	void ConnectUIwithData();

	class AActor* GetPlayerActor();

	void UIScreenChange();

	void SetZoom();

	void BindZommin();

	FVector GetCamLocation();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UASMainGameWidget> BasicHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UASMainGameWidget> SnipHUDWidgetClass;

	enum class EscreenMode
	{
		Basic,
		Sniping
	};

	void SetScreenMode(EscreenMode NewScreenMode);
	EscreenMode CurrentScreenMode = EscreenMode::Basic;
	//void UIScreenChange();

private:
	UPROPERTY()
	class UASMainGameWidget* CurMainHUDWidget;

	UPROPERTY()
	class AASCharacterBase* ControllerOwner;

	UPROPERTY()
	class UCameraComponent* SnipCam;

	UPROPERTY()
	class USceneCaptureComponent2D* SceneCaptureCam;

	UPROPERTY()
	UStaticMeshComponent* Scope;

	UPROPERTY()
	UStaticMeshComponent* SceneCameraViewPlane;

	UPROPERTY()
	class UCameraComponent* MagnificationCam;

	UPROPERTY()
	USpringArmComponent* SnipSpringArm;

	UPROPERTY()
	AASCharacterBase* PlayerCharacter;

	UPROPERTY()
	class UCameraComponent* MainCam;

	UPROPERTY()
	class UCameraComponent* SnipCam2;

	UPROPERTY()
	class ACharacter* ControlCharacter;
};
