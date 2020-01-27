// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerExpWidget.h"
#include "Components/ProgressBar.h"

void UWPlayerExpWidget::NativeConstruct()
{
	ExpBar = Cast<UProgressBar>(GetWidgetFromName("Exp_ProgressBar"));
	WRPGCHECK(nullptr != ExpBar);
}