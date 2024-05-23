// Fill out your copyright notice in the Description page of Project Settings.


#include "ASItemBox.h"
#include "Character/ASCharacterPlayer.h"
#include "UI/DetectItemWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AASItemBox::AASItemBox()
{
	//아이템 정보 읽는지 테스트 
	ItemDataName = TEXT("Bullet");
	ItemDataNum = 5;

	FString ItemDataPath = TEXT("/Game/GameData/ItemData.ItemData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ASITEM(*ItemDataPath);
	if (DT_ASITEM.Succeeded())
	{
		ASItemDataTable = DT_ASITEM.Object;
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	ItemTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ITEMTRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;
	ItemTrigger->SetupAttachment(RootComponent);
	Box->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	ItemTrigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/MarketplaceBlockout/Modern/Weapons/Assets/Rifles/01/SM_Modern_Weapons_Rifle_01.SM_Modern_Weapons_Rifle_01"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	ItemTrigger->SetCollisionProfileName(TEXT("CheckPlayer"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	UE_LOG(AS, Warning, TEXT("Make Item"));


	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	Effect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_OVERLAP(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Mobile/ICE/combat/P_Buff_ShoutFreeze_02.P_Buff_ShoutFreeze_02"));
	if (P_OVERLAP.Succeeded())
	{
		Effect->SetTemplate(P_OVERLAP.Object);
		Effect->bAutoActivate = false;
	}


	//static ConstructorHelpers::FClassFinder<UDetectItemWidget> UI_Item_C(TEXT("/Game/UI/PlayerView/WB_ItemDetect.WB_ItemDetect_C"));
	static ConstructorHelpers::FClassFinder<UDetectItemWidget> UI_Item_C(TEXT("/Game/UI/Item/WB_ItemDetect.WB_ItemDetect_C"));
	ItemWidgetClass = UI_Item_C.Class;

	canPlayerGrip = false;


}

// Called when the game starts or when spawned
void AASItemBox::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ItemWidget = CreateWidget<UDetectItemWidget>(PlayerController, ItemWidgetClass);
	
}

void AASItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AASItemBox::OnCharacterOverlap);
	//Trigger->OnComponentBeginOverlap.AddDynamic(this, &AASItemBox::OnScreenOverlap);
	//ItemTrigger->OnComponentBeginOverlap.AddDynamic(this, &AASItemBox::OnScreenOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AASItemBox::OutCharacterOverlap);
	//Trigger->OnComponentHit.AddDynamic(this, &AASItemBox::OnScreenOverlap);
	UE_LOG(AS, Warning, TEXT("Post Item"));
}

// Called every frame
void AASItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AASItemBox::SetPlayerCanGripUI()
{

}

void AASItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//ECollisionChannel CollisionChannel = OtherComp->GetCollisionObjectType();
	//if (CollisionChannel == ECC_GameTraceChannel6)
	//{
	//	ItemWidget->AddToViewport();
	//	UE_LOG(AS, Warning, TEXT("Collision with Item"));
	//}

	auto ABCharacter = Cast<AASCharacterPlayer>(OtherActor);
	if (nullptr != ABCharacter )
	{
			Effect->Activate(true);
	}
}

void AASItemBox::OutCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	//ECollisionChannel CollisionChannel = OtherComp->GetCollisionObjectType();
	//if (CollisionChannel == ECC_GameTraceChannel6)
	//{
	//	UE_LOG(AS, Warning, TEXT("Out from Collision"));
	//	ItemWidget->RemoveFromParent();
	//}
	
	Effect->Deactivate();
}

//void AASItemBox::OnScreenOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	ECollisionChannel CollisionChannel = OtherComp->GetCollisionObjectType();
//	if (CollisionChannel == ECC_GameTraceChannel6)
//	{
//		ItemWidget->AddToViewport();
//		UE_LOG(AS, Warning, TEXT("Collision with Item"));
//	}
//}

//void AASItemBox::OnScreenOverlap(UPrimitiveComponent* OverlappedCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	ECollisionChannel CollisionChannel = OtherComp->GetCollisionObjectType();
//	if (CollisionChannel == ECC_GameTraceChannel6)
//	{
//		ItemWidget->AddToViewport();
//		UE_LOG(AS, Warning, TEXT("Collision with Item"));
//	}
//}

void AASItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
}

bool AASItemBox::GetPlayerCanGrip()
{
	return canPlayerGrip;
}

void AASItemBox::SetPlayerCanGrip(bool checkPlayerCanGrip)
{
	canPlayerGrip = checkPlayerCanGrip;
}

void AASItemBox::SetItemMesh(FString itemname)
{
	
}

int AASItemBox::GetItemCount()
{
	return ItemDataNum;
}

void AASItemBox::SetItemCount(int newNum)
{
	ItemDataNum = newNum;
}

FString AASItemBox::GetItemName()
{
	return ItemDataName;
}

void AASItemBox::SetItemName(FString newName)
{
	ItemDataName = newName;
}

void AASItemBox::OnTraceHit()
{
	ItemWidget->AddToViewport();
	SetPlayerCanGrip(true);
	ItemWidget->UpdateItemName(GetItemName());
	//UE_LOG(AS, Warning, TEXT("Collision with Item"));
}

void AASItemBox::OutofTrace()
{
	ItemWidget->RemoveFromParent();
	SetPlayerCanGrip(false);
}

bool AASItemBox::GetItemData(const FString& ItemName, FItemData& OutItemData) const
{
	//if (!ItemDataTable)
	//{
	//	return false;
	//}

	//// 데이터 테이블에서 아이템을 찾음
	//FItemData* ItemDataRow = ItemDataTable->FindRow<FItemData>(FName(*ItemName), TEXT("ItemName"));
	//if (ItemDataRow)
	//{
	//	OutItemData = *ItemDataRow;
	//	return true;
	//}

	return false;
}


