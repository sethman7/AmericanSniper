// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BT/BTService_StateManager.h"
#include "AI/ASAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/ASEnemyBase.h"
#include "Components/WidgetComponent.h"

UBTService_StateManager::UBTService_StateManager()
{
	NodeName = TEXT("State_Manager");
	Interval = 0.5f;
}

void UBTService_StateManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AASAIController* AI = Cast<AASAIController>(ControllingPawn->GetController());
	AASEnemyBase* Enemy = Cast<AASEnemyBase>(ControllingPawn);

	if (AI->GetBB_IsAlert() == true)
	{
		Enemy->SetStateAnimation(EState::Alert);
		Enemy->QuestionMark->SetHiddenInGame(false);
	}

	if (AI->GetBB_IsDetect()==true)
	{
		Enemy->SetStateAnimation(EState::Chasing);
		Enemy->QuestionMark->SetHiddenInGame(true);
	}
	else 
	{
		Enemy->SetStateAnimation(EState::Idle);
	}	
}


