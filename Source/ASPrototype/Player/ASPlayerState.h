// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ASPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ASPROTOTYPE_API AASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AASPlayerState();
	
	int32 GetPlayerMaxHp();
	int32 GetPlayerCurHp();
	float GetPlayerHpratio();
	void SetPlayerCurHp(int32 ChangedPlayerHp);
	void SetBulletCurNum(int32 ChangedBulletNum);
	void SetMaganizeCurNum(int32 ChangedMaganizeNum);

	void initPlayerData();


protected:
	UPROPERTY(Transient)
	int32 MaxPlayerHp;

	UPROPERTY(Transient)
	int32 CurPlayerHp;

	UPROPERTY(Transient)
	int32 MaxNumBullet;

	UPROPERTY(Transient)
	int32 CurNumBullet;

	UPROPERTY(Transient)
	int32 MaxNumMagazine;

	UPROPERTY(Transient)
	int32 CurNumMagazine;

};
