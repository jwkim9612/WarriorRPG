// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "GameFramework/PlayerState.h"
#include "WPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExpChangedDelegate);

// PlayerState �� ��Ʈ��ũ ���ӿ��� �ſ� ������ Ŭ�����̴�. 
// �̸����� �� �뵵�� ������ �� �ֵ��� ���� �÷��̾��� ��� ���¸� �����ϰ� �ٸ� Ŭ���̾�Ʈ���� ���� ������ �� �ִ� Ư¡�� ������ �ִ�

// PlayerState ��ü�� ��� Ŭ���̾�Ʈ �ӽŰ� ���� �ӽſ��� ���� ������ �����ϸ� 
// �÷��̾� �ϳ��� �Ѱ��� PlayerState �� �Ҵ�ǰ� �����̳� �÷��̾� ��(ĳ����)�� �޸𸮿��� ����(Destroyed)�Ǵ��� 
// PlayerState �� ��� �����ϸ� �÷��̾��� ������ ������ �� �ִ�.

/**
 * 
 */
UCLASS()
class WARRIORRPG_API AWPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AWPlayerState();

private:
	struct FWPlayerData* CurrentState;

	// Transient : �𸮾� ������Ʈ���� ����ȭ ����� �־ ������Ʈ�� UPROPERTY �Ӽ��� �����ϰ� �ε��� �� �ִ�.
	// ������Ʈ ���� �� CurrentHP ���� ������ ������ ������ ����ǹǷ� �� ���� �����ϴ� ���� �ǹ̰� ����
	// ������ ������Ʈ�� ������ �� �ʿ� ���� ��ũ ������ �����Ѵ�.
	// Transient Ű���带 �߰��� �ش� �Ӽ��� ����ȭ���� ���ܽ�Ű�� ���� ����.
	UPROPERTY(Transient)
	int32 Level;

	UPROPERTY(Transient)
	float Exp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentMP;

public:
	int32 GetAttack() const;

	void InitPlayerData();
	void SetPlayerLevel(int32 NewLevel);
	void SetHP(float NewHP);
	void SetHPToDamage(float Damage);
	void AddExp(float IncomeExp);
	bool IsMaxLevel() const;
	float GetLevel() const;
	float GetCurrentHP() const;
	float GetCurrentMP() const;
	float GetHPRatio() const;
	float GetMPRatio() const;
	float GetExpRatio() const;
	struct FWPlayerData* GetCurrentStateData() const;

	FOnHPChangedDelegate OnHPChanged;
	FOnMPChangedDelegate OnMPChanged;
	FOnExpChangedDelegate OnExpChanged;
};
