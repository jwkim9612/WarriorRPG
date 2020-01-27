// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "WGameInstance.generated.h"

// GameInstance: �������� ������ �ν��Ͻ��� ���� ����� ������ ��ü.
// ���� ������ ���������� ���� �ν��Ͻ��� ���������� �ı����� �ʽ��ϴ�.
// ������ ��ȯ�Ǵ��� ������ �� �ִ� �����͸� ������ �� �ִ� ���� ��ü�Դϴ�.

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
