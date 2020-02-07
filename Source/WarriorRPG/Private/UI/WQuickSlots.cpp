// Fill out your copyright notice in the Description page of Project Settings.


#include "WQuickSlots.h"
#include "WPlayer.h"
#include "WSlot.h"
#include "blueprint/WidgetTree.h"

//void UWQuickSlot::BindQuickUpdate()
//{
//	Player->OnItemUse.AddDynamic(this, &UMyQuick::UpdateQuick);
//}

void UWQuickSlots::Init()
{
	for (int i = 0; i < 8; ++i)
	{
		//if (Player->QuickDatas[i].Type == ESlotType::SLOT_Quick)
		Player->QuickDatas[i].Texture = DefaultTexture;
	}

	// TArray<Slot> �ʱ�ȭ
	Slots.Init(nullptr, 8);

	// WidgetTree�κ��� ���� UI�� ������ �ִ� ��� widget�� �޾ƿ´�.
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	// ���񺯼�
	int ForSlotNum = 0;

	// ******** Slot�� QuickSlot���� �ٲܰ� QuickSlot Ŭ������ �ϳ� �����.

	WRPGLOG(Warning, TEXT("Init"));

	UWSlot* slot = nullptr;
	for (auto widget : widgets)
	{
		slot = Cast<UWSlot>(widget);


		if (!slot) continue;
		WRPGLOG(Warning, TEXT("widgets for"));
		slot->Player = this->Player;
		slot->SetType(ESlotType::SLOT_Quick);
		slot->Slotnum = ForSlotNum;
		slot->Init();
		Slots[slot->Slotnum] = slot;

		++ForSlotNum;
	}

}

//void UWQuickSlot::UpdateQuick()
//{
//
//	TArray<UWidget*> widgets;
//	WidgetTree->GetAllWidgets(widgets);
//
//	// ���񺯼�
//	int ForSlotNum = 0;
//
//	UWSlot* slot = nullptr;
//	for (auto widget : widgets)
//	{
//		slot = Cast<UWSlot>(widget);
//
//		if (!slot) continue;
//		slot->Player = this->Player;
//		slot->Slotnum = ForSlotNum;
//		slot->Refresh();
//
//		Slots[slot->Slotnum] = slot;
//
//		++ForSlotNum;
//	}
//}
//
//void UWQuickSlot::RefreshSlot(int slotnum)
//{
//	if (Player->Quick[slotnum].Type == EItemType::ITEM_None)
//		Player->Quick[slotnum].Texture = DefaultTexture;
//
//	Slots[slotnum]->Refresh();
//}