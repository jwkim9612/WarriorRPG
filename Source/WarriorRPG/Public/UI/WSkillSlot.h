// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WGameData.h"
#include "WSkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWSkillSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init();

public:
	class AWPlayer* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWSlot* SkillSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Description;

	struct FWSkillData SkillData;
};
