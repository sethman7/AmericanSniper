// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//인터페이스
#include "Interface_test/TestPawnNoiseInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASTestSoundBlock.generated.h"

UCLASS()
class ASPROTOTYPE_API AASTestSoundBlock : public ACharacter, public ITestPawnNoiseInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AASTestSoundBlock();

protected:
	UPROPERTY()
	class USoundBase* _sound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NoiseCheck(FVector loc, TCHAR* tag) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
