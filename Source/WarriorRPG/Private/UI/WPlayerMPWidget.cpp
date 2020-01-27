// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerMPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWPlayerMPWidget::NativeConstruct()
{
	MPBar = Cast<UProgressBar>(GetWidgetFromName("MP_ProgressBar"));
	WRPGCHECK(nullptr != MPBar);

	MPText = Cast<UTextBlock>(GetWidgetFromName("MP_TextBlock"));
	WRPGCHECK(nullptr != MPText);
}