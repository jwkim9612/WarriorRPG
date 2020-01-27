// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "GameFramework/PlayerState.h"
#include "WPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExpChangedDelegate);

// PlayerState 는 네트워크 게임에서 매우 유용한 클래스이다. 
// 이름에서 그 용도를 짐작할 수 있듯이 게임 플레이어의 모든 상태를 저장하고 다른 클라이언트에게 까지 전달할 수 있는 특징을 가지고 있다

// PlayerState 객체는 모든 클라이언트 머신과 서버 머신에서 공히 접근이 가능하며 
// 플레이어 하나에 한개의 PlayerState 가 할당되고 레벨이나 플레이어 폰(캐릭터)가 메모리에서 제거(Destroyed)되더라도 
// PlayerState 는 계속 존재하며 플레이어의 정보를 유지할 수 있다.

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

	// Transient : 언리얼 오브젝트에는 직렬화 기능이 있어서 오브젝트의 UPROPERTY 속성을 저장하고 로딩할 수 있다.
	// 컴포넌트 스탯 중 CurrentHP 값은 게임을 시작할 때마다 변경되므로 이 값을 보관하는 것은 의미가 없고
	// 오히려 오브젝트를 저장할 때 필요 없는 디스크 공간만 차지한다.
	// Transient 키워드를 추가해 해당 속성을 직렬화에서 제외시키는 것이 좋다.
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
