// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FindPatrolLocation.h"
#include "AI/ASAIController.h"
#include "Enemy/ASEnemyBase.h"
//Enemy의 PatrolPath 객체 캐스팅을 위해
#include "Enemy/PatrolPath.h"



UBTTask_FindPatrolLocation::UBTTask_FindPatrolLocation()
{
	NodeName = TEXT("FindPatrolLocation");
}

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AASEnemyBase* Enemy = Cast<AASEnemyBase>(ControllingPawn);
	APatrolPath* PatrolpathActor = Cast<APatrolPath>(Enemy->PatrolPath);
	if (PatrolpathActor == nullptr) { return EBTNodeResult::Failed; }
	ensure(PatrolpathActor);
	AASAIController* AI = Cast<AASAIController>(ControllingPawn->GetController());
	int length;
	TArray<FVector> PArray =  PatrolpathActor->PatrolPoints;
	;

	length = PArray.Num()-1;
	if (PatrolpathActor->idx <= length)
	{	
		AI->SetBB_PathLoc(PArray[PatrolpathActor->idx]);
		PatrolpathActor->idx +=1;
	}
	else
	{
		PatrolpathActor->idx = 0;
	}
	//출력 인덱스 테스트
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Name: %s || curIdx : %d "),*Enemy->Name, PatrolpathActor->idx));
	return EBTNodeResult::Succeeded;
}

