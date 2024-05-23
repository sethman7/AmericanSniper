// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_StateManager.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UBTService_StateManager : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_StateManager();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
