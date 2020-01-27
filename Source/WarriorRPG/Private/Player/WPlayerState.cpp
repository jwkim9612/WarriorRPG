// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerState.h"
#include "WGameInstance.h"

AWPlayerState::AWPlayerState()
{

}

int32 AWPlayerState::GetAttack() const
{
	return CurrentState->Attack;
}

void AWPlayerState::InitPlayerData()
{
	SetPlayerLevel(1);
	SetPlayerName(TEXT("Player"));
	Exp = 0;
}

void AWPlayerState::SetPlayerLevel(int32 NewLevel)
{
	auto WGameInstance = Cast<UWGameInstance>(GetGameInstance());
	WRPGCHECK(WGameInstance != nullptr);

	CurrentState = WGameInstance->GetPlayerData(NewLevel);
	if (CurrentState != nullptr)
	{
		Level = CurrentState->Level;
		CurrentHP = CurrentState->MaxHP;
		CurrentMP = CurrentState->MaxMP;
		OnHPChanged.Broadcast();
		OnMPChanged.Broadcast();
	}
}

void AWPlayerState::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
}

void AWPlayerState::SetHPToDamage(float Damage)
{
	WRPGCHECK(CurrentState != nullptr);
	SetHP(FMath::Clamp(CurrentHP - Damage, 0.0f, CurrentState->MaxHP));
}

void AWPlayerState::AddExp(float IncomeExp)
{
	WRPGCHECK(IncomeExp > 0);

	if (IsMaxLevel())
		return;

	Exp = Exp + IncomeExp;

	while (Exp >= CurrentState->NextExp)
	{
		Exp = Exp - CurrentState->NextExp;
		SetPlayerLevel(Level + 1);
		WRPGLOG(Warning, TEXT("Level Up : %d"), Level);

		if (IsMaxLevel())
		{
			Exp = 0;
			break;
		}
	}

	OnExpChanged.Broadcast();
}

bool AWPlayerState::IsMaxLevel() const
{
	return CurrentState->Level == 10 ? true : false;
}

float AWPlayerState::GetLevel() const
{
	return Level;
}

float AWPlayerState::GetCurrentHP() const
{
	return CurrentHP;
}

float AWPlayerState::GetCurrentMP() const
{
	return CurrentMP;
}

float AWPlayerState::GetHPRatio() const
{
	return CurrentHP / CurrentState->MaxHP;
}

float AWPlayerState::GetMPRatio() const
{
	return CurrentMP / CurrentState->MaxMP;
}

float AWPlayerState::GetExpRatio() const
{
	return Exp / CurrentState->NextExp;
}

FWPlayerData * AWPlayerState::GetCurrentStateData() const
{
	return CurrentState;
}
