// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerHPWidget.h"
#include "Components/ProgressBar.h"

void UWPlayerHPWidget::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName("HP_ProgressBar"));
	WRPGCHECK(nullptr != HPBar);
}