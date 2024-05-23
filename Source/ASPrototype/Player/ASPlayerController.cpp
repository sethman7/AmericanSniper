// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerController.h"
#include "Components/WidgetComponent.h"
#include "UI/ASMainGameWidget.h"
#include "Character/ASCharacterBase.h"
//#include "DrawDebugHelpers.h"
#include "Components/SceneCaptureComponent2D.h"


AASPlayerController::AASPlayerController()
{
	
	//static ConstructorHelpers::FClassFinder<UASMainGameWidget> UI_HUD_C(TEXT("/Game/UI/WB_GameBase_UI.WB_GameBase_UI_C"));
	static ConstructorHelpers::FClassFinder<UASMainGameWidget> UI_HUD_C(TEXT("/Game/UI/PlayerView/WB_GameBase_UI.WB_GameBase_UI_C"));
	//static ConstructorHelpers::FClassFinder<UASMainGameWidget> UI_Snip_C(TEXT("/Game/UI/WB_Sniping_UI.WB_Sniping_UI_C"));
	static ConstructorHelpers::FClassFinder<UASMainGameWidget> UI_Snip_C(TEXT("/Game/UI/PlayerView/WB_Sniping_UI.WB_Sniping_UI_C"));
	BasicHUDWidgetClass = UI_HUD_C.Class;
	SnipHUDWidgetClass = UI_Snip_C.Class;
	
}

void AASPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	ControllerOwner = Cast<AASCharacterBase>(aPawn);
}


void AASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	//SetInputMode(FInputModeGameAndUI());

	CurMainHUDWidget = CreateWidget<UASMainGameWidget>(this, BasicHUDWidgetClass);
	//CurMainHUDWidget = CreateWidget<UASMainGameWidget>(this, SnipHUDWidgetClass);
	CurMainHUDWidget->AddToViewport();

	ConnectUIwithData();
	PlayerCharacter = Cast<AASCharacterBase>(GetCharacter());
	SnipSpringArm = Cast<USpringArmComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("SnipSpringArm")));
	SnipCam = Cast<UCameraComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("SnipCam")));
	SnipCam2 = Cast<UCameraComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("SnipCam2")));
	SceneCaptureCam = Cast<USceneCaptureComponent2D>(GetCharacter()->GetDefaultSubobjectByName(TEXT("SceneCaptureComponent2D")));
	SceneCameraViewPlane = Cast<UStaticMeshComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("Plane1")));
	Scope = Cast<UStaticMeshComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("WeaponAttachment")));
	//SnipSpringArm = Cast<USpringArmComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("MagSpringArm")));
	//SnipCam = Cast<UCameraComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("OrthCamera")));
	//MagnificationCam = Cast<UCameraComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("OrthCamera")));
	MainCam = Cast<UCameraComponent>(GetCharacter()->GetDefaultSubobjectByName(TEXT("FollowCamera")));

	SceneCameraViewPlane->SetHiddenInGame(true, false);
	Scope->SetHiddenInGame(true, false);
}

UASMainGameWidget* AASPlayerController::GetHUDWidget()
{
	return CurMainHUDWidget;
}



void AASPlayerController::ConnectUIwithData()
{
	//auto CharacterWidget = Cast<UASMainGameWidget>(CurMainHUDWidget);
	//CharacterWidget->BindPlayerBase(Cast<AASCharacterBase>(ControllerOwner));
	//CharacterWidget->BindPlayerBaseForBullet(Cast<AASCharacterBase>(ControllerOwner));
	//CharacterWidget->BindPlayerBaseForMagazine(Cast<AASCharacterBase>(ControllerOwner));
	//CharacterWidget->BindPlayerBaseForItem(Cast<AASCharacterBase>(ControllerOwner));
	CurMainHUDWidget->BindPlayerBase(ControllerOwner);
	CurMainHUDWidget->BindPlayerBaseForBullet(ControllerOwner);
	CurMainHUDWidget->BindPlayerBaseForMagazine(ControllerOwner);
	CurMainHUDWidget->BindPlayerBaseForItem(ControllerOwner);
	CurMainHUDWidget->BindPlayerBaseForMagnification(ControllerOwner);
	BindZommin();
}

void AASPlayerController::SetScreenMode(EscreenMode NewScreenMode)
{
	CurrentScreenMode = NewScreenMode;
	switch (NewScreenMode)
	{
	case AASPlayerController::EscreenMode::Basic:
		CurMainHUDWidget->RemoveFromParent();
		CurMainHUDWidget = CreateWidget<UASMainGameWidget>(this, BasicHUDWidgetClass);
		ConnectUIwithData();
		CurMainHUDWidget->AddToViewport();
		ControllerOwner->InitUIData();
		//SceneCameraViewPlane->SetHiddenInGame(true, false);
		//Scope->SetHiddenInGame(true, false);
		break;
	case AASPlayerController::EscreenMode::Sniping:
		CurMainHUDWidget->RemoveFromParent();
		CurMainHUDWidget = CreateWidget<UASMainGameWidget>(this, SnipHUDWidgetClass);
		ConnectUIwithData();
		CurMainHUDWidget->AddToViewport();
		ControllerOwner->InitUIData();
		//SceneCameraViewPlane->SetHiddenInGame(false, true);
		//Scope->SetHiddenInGame(false, true);
		
		break;
	}
}



class AActor* AASPlayerController::GetPlayerActor()
{
	return ControllerOwner;
}

void AASPlayerController::UIScreenChange()
{
	switch (CurrentScreenMode)
	{
	case AASPlayerController::EscreenMode::Basic:
		SetScreenMode(EscreenMode::Sniping);
		//SetViewTarget(SnipCam);
		SnipCam->SetActive(true);
		//SnipCam2->SetActive(false);
		//MagnificationCam->SetActive(false);
		SnipSpringArm->bUsePawnControlRotation = true;
		//SnipSpringArm->bInheritPitch = false;
		//SnipSpringArm->bInheritPitch = true;
		SnipSpringArm->bDoCollisionTest = false;
		//SnipSpringArm->bInheritYaw = true;
		//SnipSpringArm->bInheritRoll = true;
		PlayerCharacter->bUseControllerRotationYaw = true;
		//bUseControllerRotationYaw = true;
		MainCam->SetActive(false);
		//SnipSpringArm1->SetActive(false);
		SceneCameraViewPlane->SetHiddenInGame(false, true);
		Scope->SetHiddenInGame(false, true);

		// Attach the CameraBoom to the specific bone of the character's skeleton
		//SnipSpringArm->AttachToComponent(ControllerOwner->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Spine_02"));
		//SnipSpringArm->SetupAttachment(ControllerOwner->GetMesh(), FName("Spine_02"));
		SnipSpringArm->SetupAttachment(ControllerOwner->GetMesh(), FName("HeadSocket"));

		break;
	case AASPlayerController::EscreenMode::Sniping:
		SetScreenMode(EscreenMode::Basic);
		SnipCam->SetActive(false);
		//MagnificationCam->SetActive(false);
		MainCam->SetActive(true);
		PlayerCharacter->bUseControllerRotationYaw = false;
		SceneCameraViewPlane->SetHiddenInGame(true, false);
		Scope->SetHiddenInGame(true, false);
		break;
	}
}

void AASPlayerController::SetZoom()
{
	SceneCaptureCam->FOVAngle = 70 - ControllerOwner->GetMagnificationratio() * 50;
	//SceneCaptureCam->SetCameraView();
	//SceneCaptureCam->SetFieldOfView(90 - ControllerOwner->GetMagnificationratio()*50);
	//MagnificationCam->SetFieldOfView(90 - ControllerOwner->GetMagnificationratio() * 50);
}

void AASPlayerController::BindZommin()
{
	ControllerOwner->NumMagnificationChanged.AddUObject(this, &AASPlayerController::SetZoom);
}

FVector AASPlayerController::GetCamLocation()
{
	return MainCam->GetComponentLocation();

}





