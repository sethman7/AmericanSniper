// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterPlayer.h"
#include "Animation/ASAnimInstance.h"
#include "Player/ASPlayerState.h"
#include "ASWeapon.h"
//카메라 헤더파일
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
//입력연결 헤더파일
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//케릭터 움직임 헤더파일
#include"GameFramework/CharacterMovementComponent.h"
//컴포넌트 충돌 헤더파일
#include "Components/CapsuleComponent.h"
//소리범위를 위한 수학공식
#include "Math/UnrealMathUtility.h"

#include "Engine/DamageEvents.h"
#include "Item/ASItemBox.h"

#include "EngineUtils.h"


AASCharacterPlayer::AASCharacterPlayer()
{

	isItemNearby = false;

	//카메라
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//SnipSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SnipSpringArm"));
	//SnipSpringArm->TargetArmLength = 50.0f;
	//SnipSpringArm->bUsePawnControlRotation = true;

	//SnipCam = CreateDefaultSubobject<UCameraComponent>(TEXT("SnipCam"));
	//SnipCam->SetupAttachment(SnipSpringArm, USpringArmComponent::SocketName);
	//SnipCam->bUsePawnControlRotation = false;

	//사운드 범위 사이즈 변수들
	MinSoundRange = 100.0f;
	MaxSoundRange = 300.0f;
	WalkSoundMultiplier = 1.0f;
	RunSoundMultiplier = 3.0f;

	// 소리 범위 컴포넌트 생성 및 설정
	SoundRangeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SoundRangeCapsule"));
	SoundRangeCapsule->SetupAttachment(RootComponent);
	SoundRangeCapsule->SetCollisionProfileName(TEXT("SoundRange")); 
	SoundRangeCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌 검사만 수행
	SoundRangeCapsule->SetCapsuleSize(50.0f, 20.0f); // 크기 설정 (반지름, 높이)
	SoundRangeCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // 상대 위치 설정

	ItemCheckSphere = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ItemCheckSphere"));
	ItemCheckSphere->SetupAttachment(RootComponent);
	ItemCheckSphere->SetCollisionProfileName(TEXT("CheckItem"));
	//ItemCheckSphere->SetCollisionProfileName(TEXT("ABCapsule"));
	ItemCheckSphere->SetCapsuleSize(10.0f,10.0f);
	ItemCheckSphere->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	ItemCheckSphere->SetRelativeScale3D(FVector(60.0f, 60.0f, 60.0f));



	ClothesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothesMesh"));
	ClothesMesh->SetupAttachment(RootComponent);
	//옷 사이즈 임시 설정
	ClothesMesh->SetRelativeLocation(FVector(- 1.0f, 6.0f, -115.0f));
	ClothesMesh->SetRelativeRotation(FRotator(0.0f, -60.0f,0.0f));
	ClothesMesh->SetRelativeScale3D(FVector(1.1f,1.1f,1.1f));

	//입력
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/ASPrototype/Input/IMC_Third.IMC_Third"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/ASPrototype/Input/IA_Jump.IA_Jump"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Game/ASPrototype/Input/IA_Move.IA_Move"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Game/ASPrototype/Input/IA_Look.IA_Look"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionWearRef(TEXT("/Game/ASPrototype/Input/IA_Wear.IA_Wear"));
	if (nullptr != InputActionWearRef.Object)
	{
		WearAction = InputActionWearRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Game/ASPrototype/Input/IA_Run.IA_Run"));
	if (nullptr != InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
	}

	SoundRangeCapsule->SetVisibility(true);
	SoundRangeCapsule->SetHiddenInGame(false);

	
	FName WeaponAttachmentSocket(TEXT("hand_rSocket_attachment"));
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponAttachmentSocket))
	{
		//WeaponAttachment = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponAttachment"));
		//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Sight(TEXT("/Game/BLS_Nomad_SMG_Content/Meshes/Attachments/SM_Reflex_Sight.SM_Reflex_Sight"));
		//if (SK_Sight.Succeeded())
		//{
		//	WeaponAttachment->SetSkeletalMesh(SK_Sight.Object);
		//}
		//WeaponAttachment->SetupAttachment(GetMesh(), WeaponAttachmentSocket);

		//if (GetMesh()->DoesSocketExist(WeaponSocket))
		//{
		//	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Rifle_01(TEXT("/Game/MarketplaceBlockout/Modern/Weapons/Assets/Rifles/01/SKM_Modern_Weapons_Rifle_01.SKM_Modern_Weapons_Rifle_01"));
		//	if (SK_Rifle_01.Succeeded())
		//	{
		//		Weapon->SetSkeletalMesh(SK_Rifle_01.Object);
		//	}
		//	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
		//}

		WeaponAttachment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponAttachment"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_Sight(TEXT("/Game/BLS_Nomad_SMG_Content/Meshes/Attachments/SM_Reflex_Sight.SM_Reflex_Sight"));
		if (SK_Sight.Succeeded())
		{
			WeaponAttachment->SetStaticMesh(SK_Sight.Object);
		}
		WeaponAttachment->SetupAttachment(GetMesh(), WeaponAttachmentSocket);
	}
	

	//무기1 세팅 
	static ConstructorHelpers::FObjectFinder<UASWeaponData> StartWeaponRef1(TEXT("/Script/ASPrototype.ASWeaponData'/Game/ASPrototype/Weapon/AssultRifle.AssultRifle'"));
	if (StartWeaponRef1.Object)
	{
		Weapon1 = Cast<UASWeaponData>(StartWeaponRef1.Object);
	}
	//무기2 세팅 
	static ConstructorHelpers::FObjectFinder<UASWeaponData> StartWeaponRef2(TEXT("/Script/ASPrototype.ASWeaponData'/Game/ASPrototype/Weapon/Sniper.Sniper'"));
	if (StartWeaponRef2.Object)
	{
		Weapon2 = Cast<UASWeaponData>(StartWeaponRef2.Object);
	}
}

void AASCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetBoolItemNearby())
	{
		MakeItemTrace();
	}

	CurrentSpeed = GetVelocity().Size();

	UpdateSoundRange();
}

bool AASCharacterPlayer::GetBoolItemNearby()
{
	return isItemNearby;
}

void AASCharacterPlayer::SetBoolItemNearby(bool newboolen)
{
	isItemNearby = newboolen;
}

void AASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AASWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon != nullptr)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		CurrentWeapon = CurWeapon;
	}

	//입력 매핑시스템을 컨트롤과 연결 
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (nullptr != Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		//Subsystem->RemoveMappingContext(DefaultMappingContext); 필요 시 연결 끊기도 가능 
	}
	UE_LOG(LogTemp, Warning, TEXT("GetController"));
	playerController = CastChecked<AASPlayerController>(GetController());
	// SoundRangeCapsule의 머티리얼을 가져옴
	UMaterialInterface* Material = SoundRangeCapsule->GetMaterial(0);

	// 머티리얼이 유효한지 확인 후 색상 변경
	if (Material)
	{
		// 색상을 빨간색으로 설정
		FLinearColor NewColor = FLinearColor::Red;

		// 머티리얼 인스턴스 생성 및 속성 설정
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), NewColor);
	
		// 머티리얼 적용
		SoundRangeCapsule->SetMaterial(0, DynamicMaterial);
	}
}

void AASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp,Warning,TEXT("SetUPInput"));
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AASCharacterBase::Shoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AASCharacterPlayer::AttackCheck);
	//PlayerInputComponent->BindAction(TEXT("SceneChange"), EInputEvent::IE_Pressed, playerController, &AASPlayerController::UIScreenChange);
	//PlayerInputComponent->BindAction(TEXT("SceneChange"), EInputEvent::IE_Pressed, this, &AASPlayerController::UIScreenChange);
	
	PlayerInputComponent->BindAction(TEXT("SceneChange"), EInputEvent::IE_Pressed, this, &AASCharacterPlayer::ChangeUI);
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AASCharacterBase::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AASCharacterBase::ZoomOut);




	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AASCharacterBase::Reload);
	PlayerInputComponent->BindAction(TEXT("Heal"), EInputEvent::IE_Pressed, this, &AASCharacterBase::Heal);
	PlayerInputComponent->BindAction(TEXT("GetDamage"), EInputEvent::IE_Pressed, this, &AASCharacterBase::TestingGetDamage);
	PlayerInputComponent->BindAction(TEXT("GetItem"), EInputEvent::IE_Pressed, this, &AASCharacterPlayer::GripItem);

	PlayerInputComponent->BindAction(TEXT("GetWeapon1"), EInputEvent::IE_Pressed, this, &AASCharacterPlayer::ChangeWeapon1);
	PlayerInputComponent->BindAction(TEXT("GetWeapon2"), EInputEvent::IE_Pressed, this, &AASCharacterPlayer::ChangeWeapon2);
	//PlayerInputComponent->BindAction(TEXT("GetWeapon2"), EInputEvent::IE_Pressed, this, &AASCharacterPlayer::ChangeWeapon(1));

	//향상된 입력 시스템 사용
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	//입력 맵핑컨텍스트에서의 액션이랑 함수랑 연결
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(WearAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::Wear);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AASCharacterPlayer::SprintStart);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AASCharacterPlayer::SprinEnd);

}

void AASCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ItemCheckSphere->OnComponentBeginOverlap.AddDynamic(this, &AASCharacterPlayer::OnItemOverlap);
	ItemCheckSphere->OnComponentEndOverlap.AddDynamic(this, &AASCharacterPlayer::OutItemOverlap);
	//ItemCheckSphere->OnComponentBeginOverlap.AddDynamic(this, OnItemOverlap);
	UE_LOG(AS, Warning, TEXT("Character Check Item"));
}

void AASCharacterPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	FString text = FString::Printf(TEXT("Enemy Find You!"));
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, text);
}

void AASCharacterPlayer::OnItemOverlap(UPrimitiveComponent* OverlappedCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetBoolItemNearby(true);
}

void AASCharacterPlayer::OutItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetBoolItemNearby(false);
}

void AASCharacterPlayer::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{	
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//if(EnemyRef)
	//{
	//	//int Hp = FMath::Clamp(GetHp() - (EnemyRef->damage), 0, 100);
	//	//SetHp(Hp);
	//	GetDamaged(EnemyRef->damage);
	//	int Hp = GetHp();
	//	FString text = FString::Printf(TEXT("Player Hp : %d"), Hp);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, text);
	//	if (Hp > 0 && Hp <= LowHp)
	//	{
	//		SetState(State::Hurt);
	//	}
	//	if (Hp <= 0)
	//	{
	//		SetDead();
	//	}
	//}
}



void AASCharacterPlayer::AttackCheck()
{
	
	FHitResult OutHit;

	FVector Location;
	FRotator Rotation;
	playerController->GetPlayerViewPoint(Location, Rotation);

	FVector ViewStart = Location;
	FVector End = ViewStart + (Rotation.Vector() * 1000);

	//FVector Weapon = CurrentWeapon->GetActorLocation();
	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetWeaponMesh();
	FVector Start = WeaponMesh->GetSocketLocation("Gun_pointSocket");
	//FVector Start = Weapon;
	//FVector Start = GetActorLocation();
	//UE_LOG(LogTemp, Error, TEXT("StartVector is %s"), *CurrentWeapon->GetActorForwardVector().ToString());
	//FVector End = ((GetActorForwardVector() * 1000.0f) + Start);

	FCollisionQueryParams CollisionParams(NAME_None,false,this);
	//FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

	bool isHit = (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams));
	//bool isHit = (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_PhysicsBody, CollisionParams));

	if (isHit)
	{
		if (OutHit.bBlockingHit)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));


		CollisionParams.bReturnPhysicalMaterial = true;
		FVector ShotDirection = OutHit.ImpactNormal;
		TSubclassOf<UDamageType> DamageType= UDamageType::StaticClass();
		FPointDamageEvent PointDamageEvent(float(GetStrength()), OutHit, ShotDirection, DamageType);
		PointDamageEvent.HitInfo = OutHit;
		UE_LOG(AS, Warning, TEXT("BoneName: %s"), *OutHit.BoneName.ToString());
		if (OutHit.BoneName == "head")
		{
			OutHit.GetActor()->TakeDamage(100, PointDamageEvent, GetController(), this);
		}
		else {
			OutHit.GetActor()->TakeDamage(GetStrength(), PointDamageEvent, GetController(), this);
		}
	}
}

void AASCharacterPlayer::MakeItemTrace()
{
	FHitResult HitResult;
	FVector Location;
	FRotator Rotation;
	playerController->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * 1000);

	//UE_LOG(LogTemp, Error, TEXT("StartVector is %s"), *CurrentWeapon->GetActorForwardVector().ToString());
	FCollisionQueryParams CollisionParams(NAME_None, false, this);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

	bool isHit = (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel6, CollisionParams));

	if (isHit)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitResult.bBlockingHit)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Trace hitting: %s"), *HitResult.GetActor()->GetName()));

		
		if (HitActor && HitActor->IsA(AASItemBox::StaticClass()))
		{
			
			AASItemBox* ItemActor = Cast<AASItemBox>(HitActor);
			if (ItemActor)
			{
				
				ItemActor->OnTraceHit();
				return;
			}
		}
	}

	for (TActorIterator<AASItemBox> It(GetWorld()); It; ++It)
	{
		AASItemBox* ItemActor = *It;
		if (ItemActor)
		{
			ItemActor->OutofTrace();
		}
	}

}

void AASCharacterPlayer::GripItem()
{
	// 플레이어 캐릭터가 E 키를 누르면 현재 위치의 아이템을 검사하여 bCanGrip이 true인 아이템의 정보를 가져옴
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AASItemBox::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		AASItemBox* Item = Cast<AASItemBox>(Actor);
		if (Item && Item->GetPlayerCanGrip())
		{
			FString ItemName = Item->GetItemName();

			// TCHAR 배열로 변환
			const TCHAR* ItemNamePtr = *ItemName;

			UE_LOG(AS, Warning, TEXT("Gripped Item: %s, Count: %d"), ItemNamePtr, Item->GetItemCount());
			FItemData ItemData;
			if (ItemName == "Bullet")
			{
				SetBulletNum(GetBulletNum() + Item->GetItemCount());
				//NumBulletChanged.Broadcast();
			}
			// 붕대 아이템을 잡았을 때
			else if (ItemName == "Bandage")
			{
				SetItemNum(GetItemNum() + Item->GetItemCount());
			}
			//FItemData ItemData;
			//if (AASItemBox::GetItemData(Item->GetName(), ItemData))
			//{
			//	// 아이템의 정보를 출력하거나 사용하는 로직을 여기에 추가
			//	UE_LOG(LogTemp, Warning, TEXT("Gripped Item: %s, Count: %d"), *Item->GetName(), Item->GetItemCount());
			//}
		}
	}
}

float AASCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	GetDamaged(FinalDamage);
	return FinalDamage;
}

void AASCharacterPlayer::ChangeWeapon(int weaponindex)
{
	switch (weaponindex)
	{
	case(1):
		ChangeWeaponMesh(Weapon1);
		break;
	case(2):
		ChangeWeaponMesh(Weapon2);
		break;
	}
}

void AASCharacterPlayer::ChangeWeapon1()
{
	ChangeWeaponMesh(Weapon1);
}

void AASCharacterPlayer::ChangeWeapon2()
{
	ChangeWeaponMesh(Weapon2);
}

void AASCharacterPlayer::ChangeWeaponMesh(UASWeaponData* NewWeaponData)
{
	if (NewWeaponData)
	{
		USkeletalMeshComponent* curWeaponMesh = CurrentWeapon->Rifle;
		curWeaponMesh->SetSkeletalMesh(NewWeaponData->WeaponModel);
	}
}

//움직임 구현
void AASCharacterPlayer::Move(const FInputActionValue& Value) 
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AASCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	USkeletalMesh* PlayerMesh = GetMesh()->GetSkeletalMeshAsset();
	//PlayerMesh->setbonetr
}

void AASCharacterPlayer::SprintStart(const FInputActionValue& Value)
{	
	GetCharacterMovement()->MaxWalkSpeed = 1000;
}

void AASCharacterPlayer::SprinEnd(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 500;
}

void AASCharacterPlayer::Wear(const FInputActionValue& Value)	
{	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(SearchMontage);

	static const FString OutfitPath = TEXT("/Game/ASPrototype/Mesh/AS_Pullover.AS_Pullover");
	// Soft Object Reference를 사용하여 옷 로드
	static TSoftObjectPtr<USkeletalMesh> NewClothesRef(OutfitPath);
	USkeletalMesh* NewClothesMesh = NewClothesRef.LoadSynchronous();

	if (NewClothesMesh)
	{
		if (ClothesMesh)
		{
			ClothesMesh->SetSkeletalMesh(NewClothesMesh);
		}
	}

	FString text = FString::Printf(TEXT("Wearing.."));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, text);
}



void AASCharacterPlayer::UpdateSoundRange()
{	
	// 현재 크기
	float Radius, Height;
	SoundRangeCapsule->GetScaledCapsuleSize(Radius, Height);
	FVector CurrentSize = FVector(Radius, Radius, Height);

	float SpeedMultiplier = CurrentSpeed > 0 ? ((GetCharacterMovement()->MaxWalkSpeed>500) ? RunSoundMultiplier : WalkSoundMultiplier) : 0.0f;
	float NewSoundRange = MinSoundRange + (MaxSoundRange - MinSoundRange) * SpeedMultiplier;
	FVector TargetSize = FVector(NewSoundRange, NewSoundRange, Height); // 높이는 고정값으로 유지
	
	// 선형 보간으로 사이즈 부드럽게 조절  _
	FVector LerpedSize = FMath::Lerp(CurrentSize, TargetSize, GetWorld()->DeltaTimeSeconds * 1.0);

	// 크기 설정
	SoundRangeCapsule->SetCapsuleSize(LerpedSize.X, LerpedSize.Y);
	
}

void AASCharacterPlayer::ChangeUI()
{
	playerController->UIScreenChange();
	auto AnimInstance = Cast<UASAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance) return;
	AnimInstance->SwitchSnipAnim();
	//SwitchSnip.Broadcast();
}








