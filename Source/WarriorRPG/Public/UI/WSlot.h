// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Blueprint/UserWidget.h"
#include "WGameData.h"
#include "WSlot.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	void SetTexture(UTexture2D* NewTexture);
	void SetType(ESlotType NewType);

	void Refresh();

public:
	class AWPlayer* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMax = 7, UIMin = 0))
	int Slotnum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Count;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ESlotType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Img;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text;

};
