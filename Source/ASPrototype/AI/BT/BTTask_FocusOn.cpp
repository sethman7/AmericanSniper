// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTTask_FocusOn.h"
#include "AI/ASAIController.h"
#include "Character/ASCharacterPlayer.h"
#include "Enemy/ASEnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

EBTNodeResult::Type UBTTask_FocusOn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AASAIController* AI = Cast<AASAIController>(ControllingPawn->GetController());
	AASEnemyBase* Enemy = Cast<AASEnemyBase>(ControllingPawn);
	AASCharacterPlayer* Player = Cast<AASCharacterPlayer>(AI->GetPlayer()); ensure(Player);

	if (Player != nullptr)
	{
		//if (GetBB_IsDetect()) { SetFocus(player); }
		FRotator EnemyRotation = Enemy->GetActorRotation();
		FVector EnemyLocation = Enemy->GetActorLocation();
		FVector AggroLocation;
		if (AI->GetBB_IsDetect()) //공격상태일 경우
		{
			AggroLocation = Player->GetActorLocation();
		}
		else //의심상태일 경우
		{
			AggroLocation = AI->GetBB_LastKnownPosition();
		}

		FRotator RotationDifferenceValue = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, AggroLocation);
		FRotator ResultValue = FRotator(0.0f, RotationDifferenceValue.Yaw ,0.0f);
		FRotator RotatorValue = FMath::RInterpTo(EnemyRotation, ResultValue,GetWorld()->GetDeltaSeconds(),0.0f);
		Enemy->SetActorRotation(RotatorValue);
		//AI->SetFocus(Player);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(" SetFocus ")));
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
