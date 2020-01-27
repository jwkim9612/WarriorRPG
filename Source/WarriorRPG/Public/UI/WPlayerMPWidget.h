// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WPlayerMPWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWPlayerMPWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	class UProgressBar* MPBar;

	UPROPERTY()
	class UTextBlock* MPText;
};
