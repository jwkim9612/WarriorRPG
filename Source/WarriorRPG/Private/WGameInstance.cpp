// Fill out your copyright notice in the Description page of Project Settings.


#include "WGameInstance.h"
#include "Engine/DataTable.h"

UWGameInstance::UWGameInstance()
{
	FString PlayerDataPath = TEXT("DataTable'/Game/Book/Data/PlayerStat.PlayerStat'");
	FString MonsterDataPath = TEXT("DataTable'/Game/Book/Data/MonsterStat.MonsterStat'");

	static ConstructorHelpers::FObjectFinder<UDataTable>
		Player_DataTable(*PlayerDataPath);

	static ConstructorHelpers::FObjectFinder<UDataTable>
		Monster_DataTable(*MonsterDataPath);

	WRPGCHECK(Player_DataTable.Succeeded());
	PlayerDataTable = Player_DataTable.Object;
	WRPGCHECK(PlayerDataTable->GetRowMap().Num() > 0);

	WRPGCHECK(Monster_DataTable.Succeeded());
	MonsterDataTable = Monster_DataTable.Object;
	WRPGCHECK(PlayerDataTable->GetRowMap().Num() > 0);
}

FWPlayerData * UWGameInstance::GetPlayerData(int32 NewLevel) const
{
	return PlayerDataTable->FindRow<FWPlayerData>(*FString::FromInt(NewLevel), TEXT(""));
}

FWMonsterData * UWGameInstance::GetMonsterData(FName NameOfMonster) const
{
	return MonsterDataTable->FindRow<FWMonsterData>(NameOfMonster, TEXT(""));
}
