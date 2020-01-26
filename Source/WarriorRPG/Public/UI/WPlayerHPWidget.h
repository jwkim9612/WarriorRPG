// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WPlayerHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWPlayerHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UProgressBar* HPBar;
};
