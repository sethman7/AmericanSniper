// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_lvl2Noise.h"
#include "Interface_test/TestPawnNoiseInterface.h"
#include "Interface_test/ASTestSoundBlock.h"
#include "AI/Sound_tags.h"

void UAnimNotify_lvl2Noise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ITestPawnNoiseInterface* NoiseInterface = Cast<ITestPawnNoiseInterface>(MeshComp->GetOwner());

	if (NoiseInterface)
	{
		FVector loc = MeshComp->GetOwner()->GetActorLocation();
		NoiseInterface->NoiseCheck(loc,tags::lvl2_Sound_tag);
	}
}
