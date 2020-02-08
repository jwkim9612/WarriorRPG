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
	virtual void OnPossess(APawn* aPawn) override;

public:
	void MonsterKill(class AWMonster* KilledMonster);

	void OnSkillTree();

	class UWPlayerHUDWidget* GetPlayerHUDWidget() const;
	class UWSkillTree* GetSkillTreeWidget() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UWPlayerHUDWidget> WPlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UWSkillTree> WSkillTreeWidgetClass;

private:
	UPROPERTY()
	class AWPlayerState* WPlayerState;

	UPROPERTY()
	class UWPlayerHUDWidget* WPlayerHUDWidget;

	UPROPERTY()
	class UWSkillTree* WSkillTreeWidget;
};
