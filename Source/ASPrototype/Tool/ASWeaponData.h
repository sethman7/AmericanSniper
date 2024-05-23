// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ASWeaponData.generated.h"


UENUM()
enum class EWeaponType
{
	Unarmed,
	Pistol,
	AssaultRifle,
	Sniper
};

UENUM()
enum class EFireMode
{
	SemiAuto,
	FullAuto
};



UCLASS()
class ASPROTOTYPE_API UASWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class USkeletalMesh> WeaponModel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponInfo)
	float DamageMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponInfo)
	int MagazineSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponInfo)
	float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponInfo)
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponInfo)
	EFireMode FireMode;
};
