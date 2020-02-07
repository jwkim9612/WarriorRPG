// Fill out your copyright notice in the Description page of Project Settings.


#include "WGameData.h"
#include "WPlayer.h"
#include "WPlayerAnimInstance.h"
#include "WPlayerState.h"

//WGameData::WGameData()
//{
//}
//
//WGameData::~WGameData()
//{
//}

void FWSkillData::Use(AWPlayer * Player)
{
	Player->GetAnimInstance()->Montage_Play(SkillAnim, 1.0f);
	Player->GetPlayerState()->SetMPToSkill(Cost);
}
