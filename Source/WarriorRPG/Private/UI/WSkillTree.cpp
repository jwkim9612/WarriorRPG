// Fill out your copyright notice in the Description page of Project Settings.


#include "WSkillTree.h"
#include "WSkillSlot.h"
#include "WSlot.h"
#include "WPlayer.h"
#include "Blueprint/WidgetTree.h"

void UWSkillTree::Init()
{
	SkillSlots.Init(nullptr, 2);

	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	int ForSkillIndex = 0;

	UWSkillSlot* SkillSlot = nullptr;
	for (auto widget : widgets)
	{
		SkillSlot = Cast<UWSkillSlot>(widget);

		if (!SkillSlot) continue;

		WRPGLOG(Warning, TEXT("For on"));
		SkillSlot->Player = this->Player;
		SkillSlot->SkillSlot->Player = this->Player;
		SkillSlot->SkillSlot->SetType(ESlotType::SLOT_Skill);
		SkillSlot->SkillSlot->Slotnum = ForSkillIndex;
		SkillSlot->SkillData = this->Player->WSkillData[ForSkillIndex];
		SkillSlot->Init();
		SkillSlots[SkillSlot->SkillSlot->Slotnum] = SkillSlot;

		++ForSkillIndex;
	}
}