// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WMonsterHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWMonsterHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class UWMonsterStatComponent> CurrentMonsterState;

public:
	void BindMonsterState(class UWMonsterStatComponent* MonsterStatComponent);
	
	void UpdateHPWidget();

public:
	UPROPERTY()
	class UProgressBar* HPBar;
};
