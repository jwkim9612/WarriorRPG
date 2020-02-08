// Fill out your copyright notice in the Description page of Project Settings.


#include "WSkillSlot.h"
#include "WSlot.h"
#include "Components/TextBlock.h"

void UWSkillSlot::Init()
{
	SkillSlot->Init();
	Name->SetText(FText::FromName(SkillData.Name));
	Description->SetText(FText::FromName(SkillData.Description));
}