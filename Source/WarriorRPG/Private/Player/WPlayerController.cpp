// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerController.h"
#include "WPlayerState.h"
#include "WMonster.h"
#include "WMonsterStatComponent.h"
#include "WPlayerHUDWidget.h"
#include "WSkillTree.h"

AWPlayerController::AWPlayerController()
{
	static ConstructorHelpers::FClassFinder<UWPlayerHUDWidget>
		UI_HUD_C(TEXT("WidgetBlueprint'/Game/Book/Player/Widget/HUDWidget.HUDWidget_C'"));

	if (UI_HUD_C.Succeeded())
	{
		WPlayerHUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UWSkillTree>
		UI_SkillTree_C(TEXT("WidgetBlueprint'/Game/Book/Player/Widget/Slot/SkillTree.SkillTree_C'"));

	if (UI_SkillTree_C.Succeeded())
	{
		WSkillTreeWidgetClass = UI_SkillTree_C.Class;
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

	WSkillTreeWidget = CreateWidget<UWSkillTree>(this, WSkillTreeWidgetClass);
	WSkillTreeWidget->AddToViewport();
	WSkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AWPlayerController::MonsterKill(AWMonster * KilledMonster)
{
	WPlayerState->AddExp(KilledMonster->GetCurrentStat()->GetDropExp());
}

void AWPlayerController::OnSkillTree()
{
	if (WSkillTreeWidget->IsVisible())
	{
		WSkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		WSkillTreeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

UWPlayerHUDWidget * AWPlayerController::GetPlayerHUDWidget() const
{
	return WPlayerHUDWidget;
}

UWSkillTree * AWPlayerController::GetSkillTreeWidget() const
{
	return WSkillTreeWidget;
}
