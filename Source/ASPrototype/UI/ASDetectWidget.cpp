// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ASDetectWidget.h"
#include "Components/ProgressBar.h"
#include "Math/UnrealMathUtility.h"
//BB������ ����
#include "AI/ASAIController.h"
#include "Components/WidgetComponent.h"


void UASDetectWidget::Onvisible()
{
	DetectBar->SetVisibility(ESlateVisibility::Visible);
}

void UASDetectWidget::OffVisible()
{
	DetectBar->SetVisibility(ESlateVisibility::Hidden);
}

void UASDetectWidget::SetRedColor()
{
	DetectBar->SetFillColorAndOpacity(FLinearColor::Red);
}

void UASDetectWidget::SetPercent(float f)
{
	DetectBar->SetPercent(f);
}

void UASDetectWidget::BlinkBar()
{
	EndDelegate.BindDynamic(this, &UASDetectWidget::OffVisible);
	if (Blink == nullptr) return;
	BindToAnimationFinished(Blink, EndDelegate);
	PlayAnimation(Blink,0.0f,3);
}



void UASDetectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DetectBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DetectProgressBar")));
	ensure(DetectBar);

	DetectBar->SetVisibility(ESlateVisibility::Hidden);

}

void UASDetectWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//timeline.TickTimeline(InDeltaTime);

	if (Owner!=nullptr) //�̰��� ���Դٴ� �� AI�� Beginplay()�� ȣ������� �ǹ���  , ��Ȯ���ʿ�
	{	
		AiRef = Cast<AASAIController>(Owner);
		//ensure(AiRef); //AI ��ü ��� ����
		MaxPercent = AiRef->MaxValue;
	}
}




void UASDetectWidget::SetAngle(float angle)
{
	SetRenderTransformAngle(angle);
}

