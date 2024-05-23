// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASCountDownWidget.h"
#include "Components/EditableText.h"
#include "Containers/UnrealString.h"
#include "Game/ASGameMode.h"
#include "Math/UnrealMathUtility.h"
void UASCountDownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Timer = Cast<UEditableText>(GetWidgetFromName(TEXT("CountDownText")));
	start = false;
	CountDown = 10.0f;
	Minus = 0.0f;
	Timer->SetText(FText::AsNumber(CountDown));
	curGameMode = Cast<AASGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ensure(curGameMode);
}

void UASCountDownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (start)
	{
		CountDown = CountDown - GetWorld()->GetDeltaSeconds();
		Timer->SetText(FText::AsNumber(FMath::Clamp(int(CountDown), 0, 10)));
		if (CountDown<=0)
		{
			start = false;
			curGameMode->GameOver();
		}
	}
}
