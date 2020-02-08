// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WGameData.h"
#include "WSkillTree.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWSkillTree : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init();

public:
	class AWPlayer* Player;

private:
	TArray<class UWSkillSlot*> SkillSlots;
};
