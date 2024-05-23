// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ASAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "ASAI.h"
#include "Sound_tags.h"

//Ž�� ���
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h" //�þ�
#include "Perception/AISenseConfig_Hearing.h" //����
#include "Perception/AISenseConfig_Touch.h"//����
#include "Perception/AISense_Touch.h"

#include "Enemy/ASEnemyCharacter.h"
#include "Character/ASCharacterPlayer.h" //SetAngle 

#include "Kismet/KismetMathLibrary.h" //charactor moving

//Ž�� ���� �߰�
#include "Components/WidgetComponent.h"
#include "UI/ASDetectWidget.h"

//���� �̿�
#include "Kismet/GameplayStatics.h"

//�׺���̼�
#include "NavigationSystem.h"

//���� Ÿ��
#include "Enemy/ASFinalTarget.h"

AASAIController::AASAIController()
{

	SetupPerception();

	//BB,BT 
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Game/ASPrototype/Enemy/AI/BB_Enemy.BB_Enemy"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}
	

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Game/ASPrototype/Enemy/AI/BT_Enemy.BT_Enemy"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_FinalTargetAssetRef(TEXT("/Game/ASPrototype/Enemy/AI/BT_FinalTarget.BT_FinalTarget"));
	if (nullptr != BT_FinalTargetAssetRef.Object)
	{
		BT_FinalTargetAsset = BT_FinalTargetAssetRef.Object;
	}



	//Detect Widget Component Setting 
	DetectBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("DetectWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> DetectBarRef(TEXT("/Game/UI/WB_DetectBar_UI.WB_DetectBar_UI_C"));
	if (DetectBarRef.Class)
	{
		DetectBar->SetWidgetClass(DetectBarRef.Class);
		DetectBar->SetWidgetSpace(EWidgetSpace::Screen);
		DetectBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		DetectBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//Init UI, State
	DetectValue =0;
	MaxValue =100;
	CurSituation = CurDetectSituation::NoneInRange;
	Alertlvl = AlertLvl::None; //�ǽɼ���
}


//Init 
void AASAIController::BeginPlay()
{
	Super::BeginPlay();
	UiRef = Cast<UASDetectWidget>(DetectBar->GetUserWidgetObject());
	ensure(UiRef);
	UiRef->SetOwner(this); 
	UiRef->AddToViewport();
	EnemyRef = Cast<AASEnemyCharacter>(GetPawn());
	ensure(EnemyRef);

	//�ٸ� Ŭ�������� �ɸ��� ��ü�� �� �޾ƿ�����, GetMesh() ȣ�� �� interface collision dataprovider����  ���� �߻�. 
	//USkeletalMeshComponent* mesh = EnemyRef->GetMesh();
	//QuestionMark->SetupAttachment(mesh);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerRef = Cast<AASCharacterPlayer>(PlayerPawn);
	SetBB_EnableRoaming(false);
}
void AASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); //���� ����  , ���ʹ��� �������� AIController�� ��� ��
	IsTarget = CheckisFinalTarget(InPawn);
	RunAI();
}
void AASAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (!UseBlackboard(BBAsset,BlackboardPtr)){	return;}
	if (IsTarget)
	{
		RunBehaviorTree(BT_FinalTargetAsset);
	}
	else
	{
		RunBehaviorTree(BTAsset);
	}

}
void AASAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}


//UI ��ġ
void AASAIController::IncreaseDetectValue()
{
	if (GetBB_IsDetect())
	{
		return;
	}

	if (DetectValue >= MaxValue)
	{
		UiRef->SetRedColor();
		UiRef->BlinkBar(); 
		CurSituation = CurDetectSituation::TargetIsDetected;
		return;
	}
	int value = 1;
	DetectValue = FMath::Clamp(DetectValue + value, 0.0f, MaxValue);
	UiRef->SetPercent(DetectValue/100.0f);
}
void AASAIController::DecreaseDetectValue()
{

	if (DetectValue <= 0)
	{
		UiRef->OffVisible();
		CurSituation = CurDetectSituation::NoneInRange;
		return;
	}
	int value = -1;
	DetectValue = FMath::Clamp(DetectValue + value, 0.0f, MaxValue);
	UiRef->SetPercent(DetectValue/100.0f);
}

//AI ���� 
void AASAIController::SetupPerception()
{
	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 700.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerComp->ConfigureSense(*SightConfig);
		AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 2000.f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerComp->ConfigureSense(*HearingConfig);
		AIPerComp->SetDominantSense(HearingConfig->GetSenseImplementation());
	}

	TouchConfig = CreateDefaultSubobject<UAISenseConfig_Touch>(TEXT("Touch Config"));
	if (TouchConfig)
	{
		AIPerComp->ConfigureSense(*TouchConfig);
		AIPerComp->SetDominantSense(TouchConfig->GetSenseImplementation());
	}


	AIPerComp->OnTargetPerceptionUpdated.AddDynamic(this, &AASAIController::On_Updated);

}
void AASAIController::On_Updated(AActor* DetectedPawn, const  FAIStimulus Stimulus)
{
	if (EnemyRef->CurState==EState::Dead){return;}

	auto SensedClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);

	//�þ� �ĺ��� ��� 
	if (SensedClass== UAISense_Sight::StaticClass())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Sight Sense")));

		AASCharacterPlayer* CurPlayer = Cast<AASCharacterPlayer>(DetectedPawn);
		CheckPlayer(CurPlayer);
	}
	//���� �ĺ��� ��� 
	else if (SensedClass == UAISense_Hearing::StaticClass() )
	{
		EventBySound = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Hearing Sense")));
		if (Stimulus.Tag == tags::lvl1_Sound_tag) // ��׷�(ex, �ֺ� ����)  + ��׷δ� �ִ� 1���� ���� 
		{
			SetAlertLvl(AlertLvl::Low);
		}
		else if (Stimulus.Tag == tags::lvl2_Sound_tag) //Player�ѱ� �Ҹ� -> �ǽɻ���(Medium) + ��׷δ� ���� ����
		{
			SetAlertLvl(AlertLvl::Medium);
			//�ι� 
		}
		else if (Stimulus.Tag == tags::lvl3_Sound_tag) //Enemy�ѱ� �Ҹ� -> �ǽɻ���(High)    
		{
			SetAlertLvl(AlertLvl::High);
			//300m�ݰ濡 �ִ� ��ó ���� �ִ� 3������ ������ġ ���� ��, �ǽɻ���(Medium)�� ���� 
		}
		LastKnownPosition = Stimulus.StimulusLocation;
		CurSituation = CurDetectSituation::TargetIsSuspected;
	}
	//���� �ĺ��� ��� 
	else if (SensedClass == UAISense_Touch::StaticClass())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Touch Sense"))); 
		SetBB_IsDetect(true);
		//��ġ�� ��밡 Player ĳ���� ���� �� ���, �ٷ� Player���� Focus On�� ��. ( UI�� ��� ���ǵ� 2�� UP )
		//ĳ���� �� ���� �Ѿ��̸� IsDetect==true
	}
}

//Ž�� ���� ���� 
void AASAIController::CheckPlayer(AActor* player)
{	
	//�ӽ� : ���� �÷��̾ �ν� (������ �ν�X)
	if (player==NULL){ return; }
	if (GetBB_Target())
	{
		SetBB_Target(nullptr); //������ �ϴ� ��Ȳ 
		StopDetection();
	}
	else
	{
		SetBB_Target(player); //���� �ϴ� ��Ȳ
		StartDetection();
	}

}
void AASAIController::StartDetection()
{
	if (!GetBB_IsDetect()){ UiRef->Onvisible(); }
	CurSituation = CurDetectSituation::TargetInRange;
}
void AASAIController::StopDetection()
{	
	if (GetBB_IsDetect() == false) //�ĺ� �ȵ� ���¿�����  
	{
		CurSituation = CurDetectSituation::TargetGetOutOfRange;
	}
}
bool AASAIController::CheckisFinalTarget(APawn* InPawn)
{
	AASFinalTarget* Target = Cast<AASFinalTarget>(InPawn);
	if (Target == nullptr)
	{
		return false;
	}
	return true;
}
void AASAIController::Tick(float DeltaTime)
{
	if (EnemyRef->CurState==EState::Dead)
	{
		StopAI();
	}

	if (CurSituation !=CurDetectSituation::NoneInRange)  
	{
		//Calculate Angle Value for UI Rotation
		FRotator ControlRotator = GetControlRotation();
		FVector PlayerLoc = PlayerRef->GetActorLocation();
		FVector EnemyLoc = EnemyRef->GetActorLocation();
			
		FRotator ControllerRotator = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
		FRotator IntervalRotator = UKismetMathLibrary::FindLookAtRotation(PlayerLoc, EnemyLoc);
		FRotator AngleRotator = UKismetMathLibrary::NormalizedDeltaRotator(IntervalRotator, ControllerRotator);
		float Angle = AngleRotator.Yaw;
		UiRef->SetAngle(Angle);
	}

	switch (CurSituation)
	{
	case CurDetectSituation::NoneInRange:
		break;

	case CurDetectSituation::TargetInRange:
		IncreaseDetectValue();
		break;

	case CurDetectSituation::TargetGetOutOfRange:
		if (DetectValue > 50)
		{	
			DetectValue = 50;
			EventBySound = false;
			CurSituation = CurDetectSituation::TargetIsSuspected;
		}
		else { DecreaseDetectValue(); }
		break;

	case CurDetectSituation::TargetIsSuspected:
		if (!EventBySound && SetAlertLvl(AlertLvl::Low))
		{
			LastKnownPosition = PlayerRef->GetActorLocation();
		}
		UiRef->OffVisible();
		SetBB_IsAlert(false); SetBB_IsAlert(true);
		CurSituation = CurDetectSituation::NoneInRange;
		break;

	case CurDetectSituation::TargetIsDetected:
		SetBB_CanShootTarget(true);
		SetBB_IsDetect(true);
		CurSituation = CurDetectSituation::NoneInRange;
		break;
	
	default:
		break;
	}

}

//��ġ ����
void AASAIController::FindNearLocation(FVector Loc, float Range)
{
	//LastKnownPosition �������� ���� ��ġ ��� 
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(EnemyRef->GetWorld());
	FNavLocation NextPosition;
	if (NavSystem->GetRandomPointInNavigableRadius(Loc, Range, NextPosition))
	{
		SetBB_LastKnownPosition(NextPosition.Location);
	}
}


//BB ������ ���� 
void AASAIController::SetBB_Target(UObject* actor)
{
	GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, actor);
}
UObject* AASAIController::GetBB_Target()
{
	return GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
}

void AASAIController::SetBB_IsDetect(bool b)
{	
	GetBlackboardComponent()->SetValueAsBool(BBKEY_IsDetect, b);
}
bool AASAIController::GetBB_IsDetect()
{	
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_IsDetect);
}

void AASAIController::SetBB_IsAlert(bool b)
{
	GetBlackboardComponent()->SetValueAsBool(BBKEY_IsAlert,b);
}
bool AASAIController::GetBB_IsAlert()
{
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_IsAlert);
}

void AASAIController::SetBB_PatrolLoc(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLLOC, vector);
}
FVector AASAIController::GetBB_PatrolLoc()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLLOC);
}

void AASAIController::SetBB_PathLoc(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_PathLOC, vector);
}
FVector AASAIController::GetBB_PathLoc()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_PathLOC);
}

void AASAIController::SetBB_AttackRange(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_AttackRange, vector);
}
FVector AASAIController::GetBB_AttackRange()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_AttackRange);
}

void AASAIController::SetBB_CanVariousActions(bool b)
{
	GetBlackboardComponent()->SetValueAsBool(BBKEY_CanVariousActions, b);
}

bool AASAIController::GetBB_CanVariousActions()
{
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_CanVariousActions);
}


void AASAIController::SetBB_IsAttack(bool b)
{
		GetBlackboardComponent()->SetValueAsBool(BBKEY_IsAttack, b);
}

bool AASAIController::GetBB_IsAttack()
{
	return  GetBlackboardComponent()->GetValueAsBool(BBKEY_IsAttack);
}

void AASAIController::SetBB_EnableRoaming(bool b)
{
	GetBlackboardComponent()->SetValueAsBool(BBKEY_EnableRoaming, b);
}
bool AASAIController::GetBB_EnableRoaming()
{
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_EnableRoaming);
}

void AASAIController::SetBB_CanShootTarget(bool b)
{
	GetBlackboardComponent()->SetValueAsBool(BBKEY_CanShootTarget, b);
}
bool AASAIController::GetBB_CanShootTarget()
{
	return GetBlackboardComponent()->GetValueAsBool(BBKEY_CanShootTarget);
}

void AASAIController::SetBB_LastKnownPosition(FVector vector)
{
	GetBlackboardComponent()->SetValueAsVector(BBKEY_LastKnownPosition,vector);
}
FVector AASAIController::GetBB_LastKnownPosition()
{
	return GetBlackboardComponent()->GetValueAsVector(BBKEY_LastKnownPosition);
}




//GetSet
UASDetectWidget* AASAIController::getWidget()
{
	return UiRef;
}

bool AASAIController::SetAlertLvl(AlertLvl NewLVl)
{
	if (NewLVl < Alertlvl) { return false; }
	Alertlvl = NewLVl;
	return true;
}

AActor* AASAIController::GetPlayer()
{	
	AActor* Actor = Cast<AActor>(PlayerRef);
	return Actor;
}

bool AASAIController::IsPlayer(AActor* actor)
{
	AASCharacterPlayer* player = Cast<AASCharacterPlayer>(actor);
	if (player)
	{
		return true;
	}
	return false;
}

//Setting RangeSize 
void AASAIController::RangeSizeDown()
{
	FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());

	auto Config = GetPerceptionComponent()->GetSenseConfig(Id);

	auto ConfigSight = Cast<UAISenseConfig_Sight>(Config);
	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
	GetPerceptionComponent()->RequestStimuliListenerUpdate();
}
void AASAIController::RangeSizeUP()
{
	FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());

	auto Config = GetPerceptionComponent()->GetSenseConfig(Id);

	auto ConfigSight = Cast<UAISenseConfig_Sight>(Config);
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
	GetPerceptionComponent()->RequestStimuliListenerUpdate();
}




























//�����ڿ����� Casting �ȵ� 
//TSubclassOf<UUserWidget> WidgetObject = DetectBar->GetWidgetClass();
//UASDetectWidget* uiRef = Cast<UASDetectWidget>(WidgetObject);
//ensure(UiRef);
//UiRef->SetOwner(this);

//ID ���� 
//AASEnemyBase* EnemyRef = Cast<AASEnemyBase>(GetPawn());
//if (EnemyRef)
//{
//	TeamId = FGenericTeamId(EnemyRef->ID);
//}


//ETeamAttitude::Type AASAIController::GetTeamAttitudeTowards(const AActor& Other) const
//{
//	return ETeamAttitude::Type();
//}
//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("DetectionLevel : %f"), DetectionLevel));
//���� �÷��̾� ���� �Ÿ� ���
//FVector Location_Between_Player_And_Enemy = EnemyLoc - PlayerLoc;
//DistanceDifference_Value = Location_Between_Player_And_Enemy.Length();
//void AASAIController::On_Updated(TArray<AActor*> const& updated_Actors)
//{
//	
//	for (size_t i = 0; i < updated_Actors.Num(); i++)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("On_Updated , EventOnBySound is %d"), EventOnBySound));
//		FActorPerceptionBlueprintInfo info;
//		AIPerComp->GetActorsPerception(updated_Actors[i], info);
//		for (size_t k = 0; k < info.LastSensedStimuli.Num(); k++)
//		{
//			FAIStimulus const stim = info.LastSensedStimuli[k];
//			if (stim.Tag==tags::lvl1_Sound_tag) //���� �Ҹ��� �ǽɻ��� 
//			{
//				EventOnBySound = true;
//				SetBB_IsAlert(true);
//				SetBB_LastKnownPosition(stim.StimulusLocation);
//			}
//			else if (stim.Tag == tags::lvl2_Sound_tag) //ū �Ҹ��� ���ݻ��� 
//			{
//				EventOnBySound = true;
//				SetBB_IsDetect(true);
//			}
//			else
//			{
//				EventOnBySound = false;
//				AASCharacterPlayer* CurPlayer = Cast<AASCharacterPlayer>(updated_Actors[i]);
//				CheckPlayer(CurPlayer);
//			}
//
//		}
//	}
//}
//void AASAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns) //�� ���� ���ֺ��� ���� �߻� 
//{	
//	for (int32 i = 0; i < DetectedPawns.Num(); ++i)
//	{
//		AASCharacterPlayer* CurPlayer = Cast<AASCharacterPlayer>(DetectedPawns[i]);
//		//ensure(CurPlayer);
//		CheckPlayer(CurPlayer);
//	}
//}