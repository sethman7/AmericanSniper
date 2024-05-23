// Fill out your copyright notice in the Description page of Project Settings.


#include "ASWeapon.h"

// Sets default values
AASWeapon::AASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Rifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIFLE"));
	RootComponent = Rifle;


	FName SightSocket(TEXT("hand_rSocket"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Rifle_One(TEXT("/Game/MarketplaceBlockout/Modern/Weapons/Assets/Rifles/01/SKM_Modern_Weapons_Rifle_01.SKM_Modern_Weapons_Rifle_01"));
	if (Rifle_One.Succeeded())
	{
		Rifle->SetSkeletalMesh(Rifle_One.Object);
	}
	
	//Rifle->SetupAttachment(getmesh)
	Rifle->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void AASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

