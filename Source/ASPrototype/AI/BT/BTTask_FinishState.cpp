// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FinishState.h"
#include "AI/ASAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/ASEnemyCharacter.h"
#include "Components/WidgetComponent.h"

EBTNodeResult::Type UBTTask_FinishState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AASEnemyBase* Enemy = Cast<AASEnemyBase>(ControllingPawn);
	ensure(Enemy);
	AASAIController* AI = Cast<AASAIController>(ControllingPawn->GetController());
	ensure(AI);
	AI->SetBB_IsAlert(false);
	AI->SetBB_IsDetect(false);
	AI->Alertlvl = AlertLvl::None;
	AI->SetBB_EnableRoaming(false);
	//블랙보드 LastKnowPostion값 초기화 하기 
	UWidgetComponent* UI = Enemy->QuestionMark;
	UI->SetHiddenInGame(true);

	AI->CurSituation = CurDetectSituation::NoneInRange;
	return EBTNodeResult::Succeeded;
}
