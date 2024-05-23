// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_Combat.h"
#include "AI/ASAIController.h"
#include "AI/ASAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/ASEnemyCharacter.h"
#include "Animation/ASAIAnimInstance.h"

UBTTask_Combat::UBTTask_Combat()
{
	NodeName = TEXT("CombatSystem");
	bNotifyTick = true;
	IsPlaying =false;
}

EBTNodeResult::Type UBTTask_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	Enemy = Cast<AASEnemyCharacter>(ControllingPawn);
	AI = Cast<AASAIController>(Enemy->GetController());

	if (Enemy==nullptr || AI==nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	Enemy->Attack();
	Enemy->CurState = EState::Attack;
	IsPlaying = true;

	Enemy->OnAttackEnd.AddLambda([this]()->void {IsPlaying = false;  Enemy->CurState = EState::Chasing; });

	return EBTNodeResult::InProgress;

}



void UBTTask_Combat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	Enemy = Cast<AASEnemyCharacter>(ControllingPawn);
	AI = Cast<AASAIController>(Enemy->GetController());
	
	if (!(IsPlaying && Enemy->CurState == EState::Attack))
	{	
		AI->SetBB_CanShootTarget(Enemy->IsPlayer);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
