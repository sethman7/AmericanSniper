// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ASEnemyCharacter.h"
#include "AI/ASAIController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Engine/EngineTypes.h"
#include "Components/SkinnedMeshComponent.h"

AASEnemyCharacter::AASEnemyCharacter()
{
	AIControllerClass = AASAIController::StaticClass();

}

float AASEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetHp(GetHp()-DamageAmount);
	return DamageAmount;
}

//void ReceivePointDamage(float Damage, const class UDamageType* DamageType, FVector HitLocation, FVector HitNormal, class UPrimitiveComponent* HitComponent, FName BoneName, FVector ShotFromDirection, class AController* InstigatedBy, AActor* DamageCauser, const FHitResult& HitInfo)
//{
//	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(HitComponent->GetName()));
//	EPhysicalSurface PhysicalSurface = UGameplayStatics::GetSurfaceType(HitInfo);
//	switch (PhysicalSurface)
//	{
//	case SurfaceType1:
//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT(" hitting: Head")));
//		break;
//	case SurfaceType2:
//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT(" hitting: ArmsAndLimb")));
//		break;
//	case SurfaceType3:
//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT(" hitting: Body")));
//		break;
//	default:
//		break;
//	}
//}

//float AASEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//EPhysicalSurface PhysicalSurface = UGameplayStatics::GetSurfaceType(HitResult);
	//FString BoneName;
	//switch (PhysicalSurface)
	//{
	//case SurfaceType1:
	//	//BoneName = FString::FromInt(PhysicalSurface);
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT(" hitting: Head")));
	//	break;
	//case SurfaceType2:
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT(" hitting: ArmsAndLimb")));
	//	break;
	//case SurfaceType3:
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT(" hitting: Body")));
	//	break;
	//default:
	//	break;
	//}

//	return DamageAmount;
//}


void AASEnemyCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
}



void AASEnemyCharacter::AttackCheck()
{
	FHitResult OutHit;
	FDamageEvent DamageEvent;
	FVector Start = GetActorLocation();//aponInfo->WeaponModel->GetComponentLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = (Start + (ForwardVector * 1000.0f));

	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(Attack), false, this);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_GameTraceChannel4, CollisionParams); //   EnemyAttack 

	if (OutHit.GetActor() == AiRef->GetPlayer())
	{

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT(" hitting: %s"),
			*OutHit.GetActor()->GetName()));
		OutHit.GetActor()->TakeDamage(10.0f, DamageEvent,GetController(),this);
		IsPlayer = true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT(" hitting: Others ")));
		IsPlayer = false;
	}
}

void AASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}






