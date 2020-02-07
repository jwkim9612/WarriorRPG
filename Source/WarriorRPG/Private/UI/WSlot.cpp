// Fill out your copyright notice in the Description page of Project Settings.


#include "WSlot.h"
#include "..\..\Public\UI\WSlot.h"
#include "WPlayer.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWSlot::Init()
{
	Refresh();
}

void UWSlot::Refresh()
{
	switch (Type)
	{
		//case ESlotType::SLOT_Item:
		//{
		//	FItemData& data = Player->Inventory[Slotnum];

		//	if (data.Texture != nullptr)
		//		SetTexture(data.Texture);

		//	Count = data.Count;

		//	if (Count <= 1)
		//		Text->SetVisibility(ESlateVisibility::Hidden);
		//	else
		//	{
		//		Text->SetVisibility(ESlateVisibility::Visible);
		//		Text->SetText(FText::FromString(FString::FromInt(Count)));
		//	}
		//	break;
	//	}
	case ESlotType::SLOT_Quick:
	{
		FSlotData& data = Player->QuickDatas[Slotnum];

		WRPGLOG(Warning, TEXT("Refresh"));

		if (data.Texture != nullptr)
			SetTexture(data.Texture);

		Count = data.Count;

		if (Count <= 1)
			Text->SetVisibility(ESlateVisibility::Hidden);
		else
		{
			Text->SetVisibility(ESlateVisibility::Visible);
			Text->SetText(FText::FromString(FString::FromInt(Count)));
		}
		break;
	}
	}
}

void UWSlot::SetTexture(UTexture2D * NewTexture)
{
	WRPGCHECK(nullptr != NewTexture);

	Img->SetBrushFromTexture(NewTexture);
}

void UWSlot::SetType(ESlotType NewType)
{
	Type = NewType;
}