// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Eqs/EnvQueryContext_Player.h"
#include "AI/ASAIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h" //FEnvQueryInstance Æ÷ÇÔ
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h" 

void UEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{	
	Super::ProvideContext(QueryInstance, ContextData);
	AASAIController * AIRef = Cast<AASAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));

	if (AIRef)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, AIRef->GetPlayer());
	}

}

