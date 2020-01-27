// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<class AWPlayerState> CurrentPlayerState;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UWPlayerHPWidget* WPlayerHPWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UWPlayerMPWidget* WPlayerMPWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UWPlayerExpWidget* WPlayerExpWidget;

public:
	void BindPlayerState(class AWPlayerState* PlayerState);

	void UpdateHPWidget();
	void UpdateMPWidget();
	void UpdateExpWidget();
};
