// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASFinalTarget.h"
#include "Game/ASGameMode.h"

AASFinalTarget::AASFinalTarget()
{

}

void AASFinalTarget::SetDead()
{
	Super::SetDead();
	AASGameMode* CurGameMode = Cast<AASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	CurGameMode->IsEscape = true;
	CurGameMode->EscapeTimerStart();
}

//¿˚ ªÁ∏¡  -->  µ®∏Æ∞‘¿Ã∆Æ »£√‚ 