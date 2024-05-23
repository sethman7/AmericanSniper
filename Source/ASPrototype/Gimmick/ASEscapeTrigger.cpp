// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ASEscapeTrigger.h"
#include "Components/BoxComponent.h"
#include "Character/ASCharacterPlayer.h"
#include "Game/ASGameMode.h"

// Sets default values
AASEscapeTrigger::AASEscapeTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName(TEXT("EscapeBox"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AASEscapeTrigger::NotifyActorEndOverlap(AActor* OtherActor)
{
	AASGameMode* CurGameMode = Cast<AASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(OtherActor);
	if (Player && CurGameMode->IsEscape==true)
	{
		CurGameMode->View_WinWidget();
	}
}

// Called when the game starts or when spawned
void AASEscapeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASEscapeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

