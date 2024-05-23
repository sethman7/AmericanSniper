// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASFinalTarget.h"
<<<<<<< HEAD
=======
#include "Game/ASGameMode.h"
>>>>>>> aa978d577c1080692cf93d18e90275be5bbfa0de

AASFinalTarget::AASFinalTarget()
{

}
<<<<<<< HEAD
=======

void AASFinalTarget::SetDead()
{
	Super::SetDead();
	AASGameMode* CurGameMode = Cast<AASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	CurGameMode->IsEscape = true;
	CurGameMode->EscapeTimerStart();
}

//Àû »ç¸Á  -->  µ¨¸®°ÔÀÌÆ® È£Ãâ 
>>>>>>> aa978d577c1080692cf93d18e90275be5bbfa0de
