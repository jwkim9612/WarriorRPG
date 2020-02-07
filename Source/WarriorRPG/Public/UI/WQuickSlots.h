// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WQuickSlots.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWQuickSlots : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//void BindQuickUpdate();
	void Init();

	//UFUNCTION()
	//void UpdateQuick();
	//void RefreshSlot(int slotnum);

public:
	class AWPlayer* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefaultTexture;

private:
	TArray<class UWSlot*> Slots;
};
