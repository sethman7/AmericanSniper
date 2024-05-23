// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Combat.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API UBTTask_Combat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Combat();
	class AASEnemyCharacter* Enemy;
	class AASAIController* AI;
	//bool IsAttacking;
	//EState *state;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type GetEndControl();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	bool IsPlaying;

};
