// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Perception/AISense.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "ASAIController.generated.h"


UENUM()
enum class CurDetectSituation
{

	NoneInRange, //�����ȿ� ���� ��Ȳ
	TargetInRange, // �����ȿ� �ִ� ��Ȳ
	TargetGetOutOfRange, // �����ȿ� ���� ���� ��Ȳ
	TargetIsSuspected, //�ǽ� �޴� ��Ȳ
	TargetIsDetected,  // �߰��� ��Ȳ
	
};

enum class AlertLvl
{
	None,
	Low, // �⺻ �ǽɻ��� , ��׷� 
	Medium, // �ֺ� �ѱ� ���� �ν��� ��� ( �ֺ� �ι� )
	High, //  ��ü �߰��� ��� , ���� �ѱ� �Ҹ��� ���� ��� ( �ֺ� ���鿡�� ���� �� �ι�, ���� ���� ���鵵 �ι� )
};


UCLASS()
class ASPROTOTYPE_API AASAIController : public AAIController
{
	GENERATED_BODY()

public:
	AASAIController();

	//Init
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void RunAI();
	void StopAI();


	//ProgressBar
	void IncreaseDetectValue();
	void DecreaseDetectValue();

	//AI ����
	UFUNCTION()
	void On_Updated(AActor* DetectedPawn, const  FAIStimulus Stimulus);
	UFUNCTION()
	void CheckPlayer(AActor* P);
	virtual void Tick(float DeltaTime) override;
	void FindNearLocation(FVector Loc,float Range);


	//Get,Set
	bool SetAlertLvl(AlertLvl NewLVl);
	AActor* GetPlayer();
	bool IsPlayer(AActor* actor);
	class UASDetectWidget* getWidget();
	


	//BB ������ ���� 
	void SetBB_LastKnownPosition(FVector vector);
	FVector GetBB_LastKnownPosition();

	void SetBB_Target(UObject* object);
	UObject* GetBB_Target();

	void SetBB_IsDetect(bool b);
	bool GetBB_IsDetect();

	void SetBB_IsAlert(bool b);
	bool GetBB_IsAlert();

	void SetBB_CanShootTarget(bool b);
	bool GetBB_CanShootTarget();

	void SetBB_EnableRoaming(bool b);
	bool GetBB_EnableRoaming();

	void SetBB_PatrolLoc(FVector vector);
	FVector GetBB_PatrolLoc();

	void SetBB_PathLoc(FVector vector);
	FVector GetBB_PathLoc();

	void SetBB_AttackRange(FVector vector);
	FVector GetBB_AttackRange();

	void SetBB_CanVariousActions(bool b);
	bool GetBB_CanVariousActions();

	void SetBB_IsAttack(bool b);
	bool GetBB_IsAttack();

	//SetRangeSize
	void RangeSizeDown();
	void RangeSizeUP();

	//UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	TObjectPtr<class UWidgetComponent> DetectBar;
	int DetectValue;
	int MaxValue;
	
	//State
	CurDetectSituation CurSituation;
	AlertLvl Alertlvl;

	//Location
	FVector LastKnownPosition;


	//virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	//void OnTargetPerceptionUpdated_Delegate(AActor* Actor, const  FAIStimulus Stimulus);
	//void On_Updated(TArray<AActor*> const& updated_Actors);


private:
	// AI Perception
	void SetupPerception();
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	class UAIPerceptionComponent* AIPerComp;
	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;
	class UAISenseConfig_Touch* TouchConfig;
	//BB,BT 
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BT_FinalTargetAsset;

	//Player, UI, enemy ��ü ĳ����
	class AASCharacterPlayer* PlayerRef;
	class UASDetectWidget* UiRef;
	class AASEnemyCharacter* EnemyRef;

	// Sight �̺�Ʈ �߻� �� ȣ��
	void StartDetection();
	void StopDetection();

	//üũ�Լ�
	bool CheckisFinalTarget(APawn* InPawn);

	//���Ǻ���
	bool EventBySound;
	bool IsTarget;

protected:
	//FGenericTeamId TeamId;
	////�� Actor�� id�� ���Ͽ�, ���� Actor�� ������ �繰���� ������ �˷��ش�. 
	//virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
