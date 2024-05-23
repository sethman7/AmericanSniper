// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface_test/ASTestSoundBlock.h"
#include "Perception/AISenseConfig_Hearing.h" 
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
AASTestSoundBlock::AASTestSoundBlock()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AASTestSoundBlock::BeginPlay()
{
	Super::BeginPlay();	
}

void AASTestSoundBlock::NoiseCheck(FVector loc, TCHAR* tag)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), _sound, loc);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, tag);
}

// Called every frame
void AASTestSoundBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AASTestSoundBlock::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

