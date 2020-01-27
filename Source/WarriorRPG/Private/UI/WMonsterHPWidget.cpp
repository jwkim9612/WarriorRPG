// Fill out your copyright notice in the Description page of Project Settings.


#include "WMonsterHPWidget.h"
#include "WMonsterStatComponent.h"
#include "Components/ProgressBar.h"

void UWMonsterHPWidget::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName("HP_ProgressBar"));

}

void UWMonsterHPWidget::BindMonsterState(UWMonsterStatComponent * MonsterStatComponent)
{
	WRPGCHECK(nullptr != MonsterStatComponent);

	CurrentMonsterState = MonsterStatComponent;

	MonsterStatComponent->OnHPChanged.AddUObject(this, &UWMonsterHPWidget::UpdateHPWidget);
}

void UWMonsterHPWidget::UpdateHPWidget()
{
	if (nullptr != HPBar)
	{
		HPBar->SetPercent(CurrentMonsterState->GetHPRatio());
	}
}
