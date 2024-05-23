//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Engine/DataAsset.h"
//#include "ASCharacterControlDataAsset.generated.h"
//
///**
// * 
// */
//UCLASS()
//class ASPROTOTYPE_API UASCharacterControlDataAsset : public UPrimaryDataAsset
//{
//	GENERATED_BODY()
//
//public:
//	// Pawn에서 사용할 데이터들
//	UPROPERTY(EditAnywhere, Category = Pawn)
//	uint8 bUseControlRotationYaw : 1;
//
//	UPROPERTY(EditAnywhere, Category = Pawn)
//	uint8 bUseControlRotationPitch : 1;
//
//	UPROPERTY(EditAnywhere, Category = Pawn)
//	uint8 bUseControlRotationRoll : 1;
//
//	//CharacterMovement에서 사용할 데이터들
//	UPROPERTY(EditAnywhere, Category = CharacterMovement)
//	FRotator RotationRate;
//
//	UPROPERTY(EditAnywhere, Category = CharacterMovement)
//	uint8 bUseControllerDesiredRotation : 1; //밑에 기능이랑 같지만 ,true일때 회전이 부드럽게 돌고 false일때 1프레임만에 빠르게 돌음
//
//	UPROPERTY(EditAnywhere, Category = CharacterMovement)
//	uint8 bOrientRotationToMovement : 1; // 이동 방향으로 자동으로 회전  
//
//	//Input
//	UPROPERTY(EditAnywhere, Category = Input)
//	TObjectPtr<class UInputMappingContext> InputMappingContext;
//
//	//Camera
//	UPROPERTY(EditAnywhere, Category = Camera)
//	float TargetArmLength;
//
//	UPROPERTY(EditAnywhere, Category = Camera)
//	FRotator RelativeRotation;
//
//	UPROPERTY(EditAnywhere, Category = Camera)
//	uint8 bUsePawnControlRotation : 1;
//
//	UPROPERTY(EditAnywhere, Category = Camera)
//	uint8 bDoCollisionTest : 1;
//
//	UPROPERTY(EditAnywhere, Category = Camera)
//	uint8 bInheritYaw : 1;
//
//	UPROPERTY(EditAnywhere, Category = Camera)
//	uint8 bInheritPitch : 1;
//
//	UPROPERTY(EditAnywhere, Category = Camera)
//	uint8 bInheritRoll : 1;
//};
