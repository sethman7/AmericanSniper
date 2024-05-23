// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "InputActionValue.h"
#include "Character/ASCharacterBase.h"
#include "Player/ASPlayerController.h"
#include "Tool/ASWeaponData.h"
#include "ASCharacterPlayer.generated.h"

/**
 * 
 */



UCLASS()
class ASPROTOTYPE_API AASCharacterPlayer : public AASCharacterBase
{
	GENERATED_BODY()
	
public:
	AASCharacterPlayer();

	UPROPERTY(EditAnywhere, Category = "Sound")
	float MinSoundRange;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float MaxSoundRange;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float WalkSoundMultiplier;

	UPROPERTY(EditAnywhere, Category = "Sound")
	float RunSoundMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UCapsuleComponent* SoundRangeCapsule;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class UCapsuleComponent* ItemCheckSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Outfit")
	class USkeletalMeshComponent* ClothesMesh;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AASWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* WeaponAttachment;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* SnipCamAttachment;

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void ChangeWeapon(int weaponindex);
	void ChangeWeapon1();
	void ChangeWeapon2();
	TObjectPtr<class UASWeaponData> Weapon1;
	TObjectPtr<class UASWeaponData> Weapon2;

public:
	void ChangeWeaponMesh(UASWeaponData* NewWeaponData);

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprinEnd(const FInputActionValue& Value);
	void Wear(const FInputActionValue& Value);
	void UpdateSoundRange();
	float CurrentSpeed;
	void ChangeUI();
	void AttackCheck();
	void MakeItemTrace();
	void GripItem();

private:
	bool isItemNearby;

public:
	bool GetBoolItemNearby();
	void SetBoolItemNearby(bool newboolen);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	//카메라 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class USpringArmComponent> SnipSpringArm;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UCameraComponent> SnipCam;



	//Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WearAction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

private:
	class AASPlayerController* playerController;

	UFUNCTION()
	void OnItemOverlap(UPrimitiveComponent* OverlappedCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OutItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
