// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerState.h"

AASPlayerState::AASPlayerState()
{
	MaxPlayerHp = 100;
	MaxNumBullet = 10;
	MaxNumMagazine = 5;
}

int32 AASPlayerState::GetPlayerMaxHp()
{
	return MaxPlayerHp;
}

int32 AASPlayerState::GetPlayerCurHp()
{
	return CurPlayerHp;
}

float AASPlayerState::GetPlayerHpratio()
{
	if (CurPlayerHp <= 0)
	{
		return 0.0f;
	}
	float HpRatio = (float)CurPlayerHp / (float)MaxPlayerHp;
	return HpRatio;
}

void AASPlayerState::SetPlayerCurHp(int32 ChangedPlayerHp)
{
	CurPlayerHp = ChangedPlayerHp;
}

void AASPlayerState::SetBulletCurNum(int32 ChangedBulletNum)
{
	CurNumBullet = ChangedBulletNum;
}

void AASPlayerState::SetMaganizeCurNum(int32 ChangedMaganizeNum)
{
	CurNumMagazine = ChangedMaganizeNum;
}

void AASPlayerState::initPlayerData()
{
	SetPlayerCurHp(MaxPlayerHp);
    SetBulletCurNum(MaxNumBullet);
	SetMaganizeCurNum(MaxNumMagazine);
}
