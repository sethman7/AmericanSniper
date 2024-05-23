// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_Noise.h"
#include "Interface_test/TestPawnNoiseInterface.h"
#include "Interface_test/ASTestSoundBlock.h"
#include "AI/Sound_tags.h"

void UAnimNotify_Noise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	//AASEnemyBase* EnemyRef = Cast<AASEnemyBase>(MeshComp->GetOwner());
	//ensure(EnemyRef);
	//AASAIController* AIRef = Cast<AASAIController>(EnemyRef->GetOwner());
	//ensure(AIRef);
	ITestPawnNoiseInterface* NoiseInterface = Cast<ITestPawnNoiseInterface>(MeshComp->GetOwner());
	ensure(NoiseInterface);

	if (NoiseInterface)
	{
		FVector loc = MeshComp->GetOwner()->GetActorLocation();
		NoiseInterface->NoiseCheck(loc,tags::lvl1_Sound_tag);
	}	
}
