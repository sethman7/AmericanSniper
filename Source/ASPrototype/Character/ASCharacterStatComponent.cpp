// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterStatComponent.h"
#include "Game/ASGameInstance.h"

// Sets default values for this component's properties
UASCharacterStatComponent::UASCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UASCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = 100;
	// ...
	
}

void UASCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame


