// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_Shooting.h"
#include "Interface/ASAttackCheckInterface.h"

void UAnimNotify_Shooting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	IASAttackCheckInterface* AttackCheckInterface = Cast<IASAttackCheckInterface>(MeshComp->GetOwner());

	if (AttackCheckInterface)
	{
		AttackCheckInterface->AttackCheck();
	}
}
