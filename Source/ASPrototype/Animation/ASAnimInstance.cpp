// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ASAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UASAnimInstance::UASAnimInstance()
{
	MoveThreadshold = 3.0f;
	JumpeThreadshold = 30.0f;
	DoSniping = false;
}

void UASAnimInstance::StateHandler(State NewState)
{	
	switch (NewState)
	{
	case State::None:
		break;
	case State::Prone:
		break;
	case State::Crawl:
		break;
	case State::Hurt:
		bIsHurt = 1;
		UE_LOG(LogTemp, Log, TEXT("HURT"));
	case State::Hidden:
		break;
	case State::Dead:
		break;
	default:
		break;
	}
}

void UASAnimInstance::SwitchSnipAnim()
{
	if (DoSniping)
	{
		DoSniping = false;
	}
	else DoSniping = true;
}

//임시 함수
//void UASAnimInstance::SetCurrentHp(int NewHp)
//{
//	Hp = NewHp;
//}

void UASAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	//Actor = Cast<AASCharacterBase>(Owner);
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	//OnHpChanged.AddUObject(this, &UASAnimInstance::SwitchSnipAnim);
}

void UASAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		GroundSpeed = Movement->Velocity.Size2D(); 
		bIsIdle = GroundSpeed < MoveThreadshold;

		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling && (Movement->Velocity.Z > JumpeThreadshold);

		bIsRunning = (Owner->GetCharacterMovement()->MaxWalkSpeed > 500) && !bIsIdle && !bIsJumping;

		OwnerLocation = Owner->GetActorLocation();

		//bIsHurt = Hp < 50;
		//UE_LOG(AS, Log, TEXT("Character Location :: %s"), Owner->GetActorLocation().ToString());
		Owner = Cast<ACharacter>(GetOwningActor());
		AController* OwningController = Owner->GetController();
		if (OwningController)
		{
			ControlRotation = OwningController->GetControlRotation();
			//FRotator ActorRotation = Owner->GetActorRotation();

			//LookAtYaw = FRotator::NormalizeAxis(ControlRotation.Yaw - ActorRotation.Yaw);
			LookAtPitch = ControlRotation.Pitch - 280.0f;
			//UE_LOG(AS, Warning, TEXT("Pitch : %f"), ControlRotation.Pitch);

		}
	}
}


