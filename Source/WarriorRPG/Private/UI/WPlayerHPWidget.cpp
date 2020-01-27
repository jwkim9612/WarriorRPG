// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerHPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWPlayerHPWidget::NativeConstruct()
{
	HPBar = Cast<UProgressBar>(GetWidgetFromName("HP_ProgressBar"));
	WRPGCHECK(nullptr != HPBar);

	HPText = Cast<UTextBlock>(GetWidgetFromName("HP_TextBlock"));
	WRPGCHECK(nullptr != HPText);
}