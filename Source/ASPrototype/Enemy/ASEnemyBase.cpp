// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//BB정보 얻기 위해 
#include "AI/ASAIController.h"
#include "UI/ASDetectWidget.h"
#include "Tool/ASWeaponData.h"
#include "Tool/ASWeaponItem.h"
#include "Components/WidgetComponent.h"

// Sets default values
AASEnemyBase::AASEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ASEnemy"));

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	//GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//Widget
	QuestionMark = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestionMarkWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> QuestionMarkRef(TEXT("/Game/UI/WB_QuestionMark_UI.WB_QuestionMark_UI_C"));
	ensure(QuestionMarkRef.Class);
	if (QuestionMarkRef.Class)
	{
		QuestionMark->SetWidgetClass(QuestionMarkRef.Class);
		QuestionMark->SetWidgetSpace(EWidgetSpace::Screen);
		QuestionMark->SetDrawSize(FVector2D(30.0f, 30.0f));
		QuestionMark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		QuestionMark->SetupAttachment(GetMesh());
		QuestionMark->AddRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		QuestionMark->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));
		QuestionMark->SetHiddenInGame(true);
	}


	//Stats
	MaxHp = 100;
	CurHp = 100;
	Damage = 10;
	CurState = EState::Idle; 

	//스켈레톤 + 애니메이션 적용 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharaterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ASPrototype/Enemy/Enemy/Meshes/SK_HeavyGSoldier_simple.SK_HeavyGSoldier_simple'"));
	if (CharaterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharaterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ASPrototype/Enemy/Enemy/ABP_TempEnemy.ABP_TempEnemy_C'"));
	//ensure(AnimInstanceClassRef.Class);
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}




	//Weapon 
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CurrentWeapon"));
	WeaponMesh->SetupAttachment(GetMesh(), FName(TEXT("Weapon_Socket")));

	//무기1 세팅 
	static ConstructorHelpers::FObjectFinder<UASWeaponData> StartWeaponRef1(TEXT("/Script/ASPrototype.ASWeaponData'/Game/ASPrototype/Weapon/AssultRifle.AssultRifle'"));
	if (StartWeaponRef1.Object)
	{
		Weapon1 = Cast<UASWeaponData>(StartWeaponRef1.Object);
		
		EquipWeapon(Weapon1);
	}
	//무기2 세팅 
	static ConstructorHelpers::FObjectFinder<UASWeaponData> StartWeaponRef2(TEXT("/Script/ASPrototype.ASWeaponData'/Game/ASPrototype/Weapon/Sniper.Sniper'"));
	if (StartWeaponRef2.Object)
	{
		Weapon2 = Cast<UASWeaponData>(StartWeaponRef2.Object);
	}


	//Speed
	WalkSpeed = 300.0f;
	RunSpeed = 500.0f;
}

uint32 AASEnemyBase::GetHp()
{
	return CurHp;
}

void AASEnemyBase::SetHp(uint32 Hp)
{
	if (Hp<=0 && CurState!= EState::Dead)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("Enemy Dead")));
		CurState = EState::Dead;  
		Dead();
		return;
	}
	CurHp = Hp;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString::Printf(TEXT("EnemyHp : %d"), CurHp));
}

void AASEnemyBase::Dead()
{
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->StopAllMontages(0.0f);
	//const float DelayTime = AnimInstance->Montage_Play(DeadMontage);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//SetActorEnableCollision(false);
	//Deda Animation Start
	const float DelayTime =PlayAnimMontage(DeadMontage);
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), DelayTime-0.5, false);
}

void AASEnemyBase::EquipWeapon(UASWeaponData* NewWeaponData)
{
	if (NewWeaponData)
	{
		WeaponMesh->SetSkeletalMesh(NewWeaponData->WeaponModel);
	}
}


void AASEnemyBase::Attack()
{
	const float DelayTime = PlayAnimMontage(AttackMontage);
	AttackEnd(DelayTime);
}

void AASEnemyBase::AttackEnd(const float InDelayTime)
{
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			OnAttackEnd.Broadcast();
			// 타이머 초기화
			GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
		}), InDelayTime, false);
}



// Called when the game starts or when spawned
void AASEnemyBase::BeginPlay()
{	
	Super::BeginPlay();
	AiRef = Cast<AASAIController>(GetOwner());
}

// Called every frame
void AASEnemyBase::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AASEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AASEnemyBase::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetMesh()->GetSocketLocation("HeadSocket");
	OutRotation = GetMesh()->GetSocketRotation("HeadSocket");
}

void AASEnemyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void AASEnemyBase::SetState(EState NewState)
{
	CurState = NewState;
	SetStateAnimation(CurState);
}

EState AASEnemyBase::GetState()
{
	return EState();
}


void AASEnemyBase::SetStateAnimation(EState NewState)
{
	AiRef = Cast<AASAIController>(GetOwner());
	switch (NewState)
	{	
	case EState::Idle:
		//WeaponInfo->WeaponModel->SetHiddenInGame(true);
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		//AiRef->RangeSizeDown();
		break;
	case EState::Alert:
		EquipWeapon(Weapon2);
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
		break;
	case EState::Chasing:
		//WeaponInfo->WeaponModel->SetHiddenInGame(false);
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed; // 상태변화에서 가장 의미
		//AiRef->RangeSizeUP();
		break;

	case EState::Attack:
		//WeaponInfo->WeaponModel->SetHiddenInGame(false);
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

		//AiRef->RangeSizeUP();
		break;

	case EState::Hurt:
		break;

	case EState::Hidden:
		break;

	case EState::Dead:
		break;

	default:
		break;
	}
}


