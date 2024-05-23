// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/ASGameMode.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ASItemBox.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData():ItemName(TEXT(".")), MeshPath(TEXT(".")) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshPath;

	// 추가적인 아이템 속성들을 정의할 수 있음
};

UCLASS()
class ASPROTOTYPE_API AASItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* ItemTrigger;


	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;

	void SetPlayerCanGripUI();

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OutCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UFUNCTION()
	//void OnScreenOverlap(UPrimitiveComponent* OverlappedCom, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//void OnScreenOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UDetectItemWidget> ItemWidgetClass;

	UPROPERTY()
	class UDetectItemWidget* ItemWidget;

private:
	APlayerController* PlayerController;

	bool canPlayerGrip;

	int ItemDataNum;

	FString ItemDataName;

	UPROPERTY()
	UDataTable* ASItemDataTable;

public:
	bool GetPlayerCanGrip();
	void SetPlayerCanGrip(bool checkPlayerCanGrip);
	void SetItemMesh(FString itemname);

	int GetItemCount();
	void SetItemCount(int newNum);

	FString GetItemName();
	void SetItemName(FString newName);

	void OnTraceHit();
	void OutofTrace();

	bool GetItemData(const FString& ItemName, FItemData& OutItemData) const;

};
