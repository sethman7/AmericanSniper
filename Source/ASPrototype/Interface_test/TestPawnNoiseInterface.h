// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestPawnNoiseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTestPawnNoiseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASPROTOTYPE_API ITestPawnNoiseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void NoiseCheck(FVector loc, TCHAR* tag) = 0;
};
