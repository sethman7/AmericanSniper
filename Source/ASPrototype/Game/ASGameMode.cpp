// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ASGameMode.h"
#include "Player/ASPlayerState.h"
#include "Blueprint/UserWidget.h"

//액터 찾기
#include "Enemy/ASFinalTarget.h"
#include "Character/ASCharacterPlayer.h"

//타이머 
#include "UI/ASCountDownWidget.h"

#include "UI/ASCountDownWidget.h"
#include "Components/EditableTextBox.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(AS);

AASGameMode::AASGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/ASPrototype/BluePrint/BP_Player.BP_Player_C'"));
	//DefaultPawnClass Setting
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	//PlayerControllerClass Setting
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ASPrototype.ASPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	PlayerStateClass = AASPlayerState::StaticClass();
	//PlayerStateClass = PlayerStateClass;


	static ConstructorHelpers::FClassFinder<UUserWidget> CountDownWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WB_CountDown.WB_CountDown_C'"));
	if (CountDownWidgetRef.Class)
	{
		ensure(CountDownWidgetRef.Class);
		CountDownWidgetClass = CountDownWidgetRef.Class;
	}
	ensure(CountDownWidgetClass);
	IsEscape = false; 

}


void AASGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AASGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto ASPlayerState = Cast<AASPlayerState>(NewPlayer->PlayerState);
	ASPlayerState->initPlayerData();
}

void AASGameMode::EscapeTimerStart()
{
	CountDownWidget->AddToViewport();
	CountDownWidget->start = true;
}

void AASGameMode::View_WinWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("You WIN!")));
}

void AASGameMode::View_LoseWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("You Loser!")));
}

void AASGameMode::GameOver()
{
	Player->SetDead();
	View_LoseWidget();
}


void AASGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);

	CountDownWidget= Cast<UASCountDownWidget>(CreateWidget(GetWorld(), CountDownWidgetClass));
	ensure(CountDownWidget);
	//Get Player
	Player = Cast<AASCharacterPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AASCharacterPlayer::StaticClass()));
	ensure(Player);
	//Get Target
	Target = Cast<AASFinalTarget>(UGameplayStatics::GetActorOfClass(GetWorld(), AASFinalTarget::StaticClass()));
	ensure(Target);



}

void AASGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


