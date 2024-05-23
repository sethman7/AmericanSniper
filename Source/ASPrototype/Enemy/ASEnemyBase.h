// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tool/ASWeaponData.h"
#include "Tool/ASWeaponItem.h"
//�̵� ���� ����
//#include "Enemy/PatrolPath.h"
//�� ���ʹ̵��� �Ǻ��ϱ� ���� ID���� �־���
//#include "GenericTeamAgentInterface.h"

#include "ASEnemyBase.generated.h"

UENUM()
enum class EState
{
	Idle, 
	Alert, //�ǽ� ����
	Chasing, //�Ѵ� ����
	Attack, //���ݻ��� 
	Hurt,  //���ҰŸ���
	Hidden, //���� ����
	Dead //���
}; 

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);


UCLASS()
class ASPROTOTYPE_API AASEnemyBase : public ACharacter//, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AASEnemyBase();

	FOnAttackEndDelegate OnAttackEnd;

	float SplineSpeed;
	float DistanceAlongSpline;

	uint32 GetHp();
	void SetHp(uint32 Hp);
<<<<<<< HEAD
	void Dead();

=======
	virtual void SetDead();
	void RandomActions();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
>>>>>>> aa978d577c1080692cf93d18e90275be5bbfa0de
private:
	uint32 MaxHp;
	uint32 CurHp;
	uint8 Damage;


public:	
	EState CurState;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void SetStateAnimation(EState NewState);

	//TArray<FVector> ���� ���� Actor Ŭ���� ������ 
	UPROPERTY(EditAnywhere)
	TObjectPtr<class APatrolPath> PatrolPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

<<<<<<< HEAD
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ReloadMontage;

>>>>>>> aa978d577c1080692cf93d18e90275be5bbfa0de



	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;
	TObjectPtr<class UASWeaponData> Weapon1;
	TObjectPtr<class UASWeaponData> Weapon2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = name)
	FString Name;

	void EquipWeapon(UASWeaponData* NewWeaponData);





	void Attack();

	void AttackEnd(const float InDelyTime);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	//int32 ID = 0;

	//���ʹ̰� �������� �������� �ƴ��� �Ǵ��ϱ� ���� ����
	//virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UWidgetComponent> QuestionMark;



protected:
	class AASAIController* AiRef;
	class UASDetectWidget* UiRef;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���ʹ̰� �������� �������� �ƴ��� �Ǵ��ϱ� ���� ���� 
	//FGenericTeamId TeamId;

	void SetState(EState NewState);
	EState GetState();

};

