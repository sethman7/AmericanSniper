// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Perception/AISense.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "ASAIController.generated.h"


UENUM()
enum class CurDetectSituation
{

	NoneInRange, //범위안에 없는 상황
	TargetInRange, // 범위안에 있는 상황
	TargetGetOutOfRange, // 범위안에 들어갔다 나간 상황
	TargetIsSuspected, //의심 받는 상황
	TargetIsDetected,  // 발각된 상황
	
};

enum class AlertLvl
{
	None,
	Low, // 기본 의심상태 , 어그로 
	Medium, // 주변 총기 사운드 인식할 경우 ( 주변 로밍 )
	High, //  시체 발견한 경우 , 적의 총기 소리를 들을 경우 ( 주변 적들에게 연락 후 로밍, 연락 받은 적들도 로밍 )
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

	//AI 관련
	UFUNCTION()
	void On_Updated(AActor* DetectedPawn, const  FAIStimulus Stimulus);
	UFUNCTION()
	void CheckPlayer(AActor* P);
	virtual void Tick(float DeltaTime) override;
	void FindNearLocation(FVector Loc,float Range);


	//Get,Set
	bool SetAlertLvl(AlertLvl NewLVl);
	AActor* GetPlayer();
	class UASDetectWidget* getWidget();
	


	//BB 데이터 정보 
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

	//BB,BT 
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BT_FinalTargetAsset;

	//Player, UI, enemy 객체 캐스팅
	class AASCharacterPlayer* PlayerRef;
	class UASDetectWidget* UiRef;
	class AASEnemyCharacter* EnemyRef;

	// Sight 이벤트 발생 시 호출
	void StartDetection();
	void StopDetection();

	//체크함수
	bool CheckisFinalTarget(APawn* InPawn);

	//조건변수
	bool EventBySound;
	bool IsTarget;

protected:
	//FGenericTeamId TeamId;
	////각 Actor의 id를 비교하여, 현재 Actor가 적인지 사물인지 팀인지 알려준다. 
	//virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
