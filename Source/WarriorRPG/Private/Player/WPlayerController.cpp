// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerController.h"
#include "WPlayerState.h"
#include "WMonster.h"
#include "WMonsterStatComponent.h"
#include "WPlayerHUDWidget.h"

AWPlayerController::AWPlayerController()
{
	static ConstructorHelpers::FClassFinder<UWPlayerHUDWidget>
		UI_HUD_C(TEXT("WidgetBlueprint'/Game/Book/Player/Widget/HUDWidget.HUDWidget_C'"));

	if (UI_HUD_C.Succeeded())
	{
		WPlayerHUDWidgetClass = UI_HUD_C.Class;
	}
}

void AWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	WPlayerState = Cast<AWPlayerState>(PlayerState);
	WRPGCHECK(WPlayerState);
	WPlayerHUDWidget->BindPlayerState(WPlayerState);
}

void AWPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	WPlayerHUDWidget = CreateWidget<UWPlayerHUDWidget>(this, WPlayerHUDWidgetClass);
	WPlayerHUDWidget->AddToViewport();
}

void AWPlayerController::MonsterKill(AWMonster * KilledMonster)
{
	WPlayerState->AddExp(KilledMonster->GetCurrentStat()->GetDropExp());
}
