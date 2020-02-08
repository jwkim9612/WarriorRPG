// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "WarriorRPG.h"
#include "WGameData.generated.h"
//
///**
// * 
// */
//class WARRIORRPG_API WGameData
//{
//public:
//	WGameData();
//	~WGameData();
//};

UENUM(BlueprintType)
enum class Pressed : uint8
{
	Press_None	UMETA(DisplayName = "None"),
	Press_Up	UMETA(DisplayName = "Up"),
	Press_Down	UMETA(DisplayName = "Down"),
	Press_Left	UMETA(DisplayName = "Left"),
	Press_Right	UMETA(DisplayName = "Right")
};

enum class AttackMode : uint8
{
	Default,
	Focus
};

enum class AttackType : uint8
{
	Default,
	Skill
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	SLOT_None		UMETA(DisplayName = "None"),
	SLOT_Item		UMETA(DisplayName = "Item"),
	SLOT_Skill		UMETA(DisplayName = "Skill"),
	SLOT_Quick		UMETA(DisplayName = "Quick"),
	SLOT_Q_Item		UMETA(DisplayName = "Q_Item"),
	SLOT_Q_Skill	UMETA(DisplayName = "Q_Skill")
};

USTRUCT(BlueprintType)
struct FWSkillData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Index;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Textrue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Cost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Scale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* SkillAnim;

public:

	void Use(class AWPlayer* Player);
};

USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_BODY()

public:
	FSlotData() : Name(TEXT("")), Texture(nullptr), Type(ESlotType::SLOT_None), Count(0) {};

	//void Clear(AMHPlayer* Player);
	//void Use(AMHPlayer* Player);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count;
};