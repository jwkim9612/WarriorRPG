// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "WGameInstance.generated.h"

// GameInstance: 실행중인 게임의 인스턴스를 위한 고수준 관리자 개체.
// 게임 생성시 생성되지만 게임 인스턴스가 닫힐때까지 파괴되지 않습니다.
// 레벨이 전환되더라도 공유할 수 있는 데이터를 저장할 수 있는 전역 개체입니다.

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWPlayerData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FWPlayerData() : Level(1), Attack(10), MaxHP(100.0f), MaxMP(10.0f), NextExp(30.0f) { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float NextExp;
};

USTRUCT(BlueprintType)
struct FWMonsterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FWMonsterData() : MaxHP(1), Attack(1), DropExp(1) { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DropExp;
};

UCLASS()
class WARRIORRPG_API UWGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UWGameInstance();

	UPROPERTY()
	UDataTable* PlayerDataTable;

	UPROPERTY()
	UDataTable* MonsterDataTable;

public:
	FWPlayerData* GetPlayerData(int32 NewLevel) const;
	FWMonsterData* GetMonsterData(FName NameOfMonster) const;
};
