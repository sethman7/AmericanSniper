// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ASAIAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UASAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy)
	bool bIsIdle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float Speed;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
