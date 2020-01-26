// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerController.h"
#include "WPlayerState.h"
#include "WMonster.h"
#include "WMonsterStatComponent.h"

AWPlayerController::AWPlayerController()
{

}

void AWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	WPlayerState = Cast<AWPlayerState>(PlayerState);
}

void AWPlayerController::MonsterKill(AWMonster * KilledMonster)
{
	WPlayerState->AddExp(KilledMonster->GetCurrentStat()->GetDropExp());
}
