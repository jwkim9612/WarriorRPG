// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WPlayerExpWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWPlayerExpWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	class UProgressBar* ExpBar;
};
