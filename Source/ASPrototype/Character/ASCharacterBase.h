// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ASCharacterBase.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnStateChangeDelegate);

UENUM()
enum class State
{
	None,
	Prone, //�ɱ�
	Crawl, //���帮��
	Hurt,  //���ҰŸ���
	Hidden, //���� ����
	Dead
};


UCLASS()
class ASPROTOTYPE_API AASCharacterBase : public ACharacter
{
	GENERATED_BODY()

private:
	int Curstrength;
	uint32 MaxHp;
	uint32 CurHp;
	uint32 MaxBulletNum;
	uint32 CurBulletNum;
	uint32 MaxMagazineNum;
	uint32 CurMagazineNum;
	uint32 MaxItemNum;
	uint32 CurItemNum;
	uint8 Damage;
	State CurState;
	//UPROPERTY()
	//class AASPlayerController* PlayerController;
	int MaxMagnification;
	int CurMagnification;


public:
	// Sets default values for this character's properties
	AASCharacterBase();
	virtual void PostInitializeComponents() override;
	void SetDead();
	int GetHp();
	int GetBulletNum();
	int GetMagazineNum();
	int GetItemNum();
	int GetStrength();
	float GetHpratio();
	void SetHp(int Hp);
	void SetBulletNum(int Num);
	void SetMagazineNum(int Num);
	void SetItemNum(int Num);
	void GetDamaged(int damage);
	void SetStrength(int newStrength);
	bool AttackCheck();


	void SetState(State NewState);
	void Shoot();
	void Reload();
	void Heal();
	void TestingGetDamage();
	void InitUIData();
	State GetState();
	FOnStateChangeDelegate OnHpChanged;
	FOnStateChangeDelegate NumBulletChanged;
	FOnStateChangeDelegate NumMagazineChanged;
	FOnStateChangeDelegate NumItemChanged;
	FOnStateChangeDelegate NumMagnificationChanged;
	FOnStateChangeDelegate SwitchSnip;

	int GetMagnificationNum();
	void SetMagnificationNum(int newMag);
	float GetMagnificationratio();
	void ZoomIn();
	void ZoomOut();


	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UASCharacterStatComponent* CharacterStat;
	

protected:
	virtual void BeginPlay() override;

	uint8 LowHp; // LowHp ���ϸ� ���ҰŸ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SearchMontage;

	FTimerHandle DeadTimerHandle;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Controller)
	//TSubclassOf<class AASPlayerController> CurplayerControllerClass;

	virtual void Tick(float DeltaTime) override;
};
