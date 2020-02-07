// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_MULTICAST_DELEGATE(FOnCanNextAttackDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHitAttackDelegate);

#include "WarriorRPG.h"
#include "Animation/AnimInstance.h"
#include "WGameData.h"
#include "WPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UWPlayerAnimInstance();

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	bool IsInAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	bool IsFocusMode = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	Pressed PressKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *GroundAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *AirAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *SkillMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *Emote1Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *Emote2Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *Emote3Montage;

	UFUNCTION()
	void AnimNotify_CanNextAttack();

	UFUNCTION()
	void AnimNotify_HitAttack();

public:
	void PlayGroundAttackMontage();
	void PlayAirAttackMontage();
	void PlaySkillMontage();
	void JumpToNextAttackSection(int32 NewSection);
	FName GetAttackMontageSectionName(int32 Section);

	void PlayEmote(int32 EmoteNum);
	void StopMontage();

	FOnCanNextAttackDelegate OnCanNextAttack;
	FOnHitAttackDelegate OnHitAttack;
};
