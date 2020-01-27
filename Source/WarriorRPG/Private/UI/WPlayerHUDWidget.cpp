// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerHUDWidget.h"
#include "WGameInstance.h"
#include "WPlayerState.h"
#include "WPlayerHPWidget.h"
#include "WPlayerMPWidget.h"
#include "WPlayerExpWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "..\..\Public\UI\WPlayerHUDWidget.h"

void UWPlayerHUDWidget::BindPlayerState(AWPlayerState * PlayerState)
{
	WRPGCHECK(nullptr != PlayerState);

	CurrentPlayerState = PlayerState;
	PlayerState->OnHPChanged.AddUObject(this, &UWPlayerHUDWidget::UpdateHPWidget);
	PlayerState->OnMPChanged.AddUObject(this, &UWPlayerHUDWidget::UpdateMPWidget);
	PlayerState->OnExpChanged.AddUObject(this, &UWPlayerHUDWidget::UpdateExpWidget);
}

void UWPlayerHUDWidget::UpdateHPWidget()
{
	WPlayerHPWidget->HPBar->SetPercent(CurrentPlayerState->GetHPRatio());
	WPlayerHPWidget->HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentPlayerState->GetCurrentHP(), CurrentPlayerState->GetCurrentStateData()->MaxHP)));
}

void UWPlayerHUDWidget::UpdateMPWidget()
{
	WPlayerMPWidget->MPBar->SetPercent(CurrentPlayerState->GetMPRatio());
	WPlayerMPWidget->MPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentPlayerState->GetCurrentMP(), CurrentPlayerState->GetCurrentStateData()->MaxMP)));
}

void UWPlayerHUDWidget::UpdateExpWidget()
{
	if (CurrentPlayerState->IsMaxLevel())
	{
		WPlayerExpWidget->ExpBar->SetPercent(1);
	}
	else
	{
		WPlayerExpWidget->ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	}
}
