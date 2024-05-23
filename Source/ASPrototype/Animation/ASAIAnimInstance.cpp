// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ASAIAnimInstance.h"

void UASAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	bIsIdle = true;
}

void UASAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AActor* pawn = GetOwningActor(); 
	ensure(pawn);
	if (pawn != nullptr)
	{
		Speed = pawn->GetVelocity().Size();
	}

	if (Speed>0.0f)
	{
		bIsIdle = false;
	}
	else
	{
		bIsIdle = true;
	}
}
