// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "GameFramework/PlayerController.h"
#include "WPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API AWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	void MonsterKill(class AWMonster* KilledMonster);

private:
	class AWPlayerState* WPlayerState;
};
