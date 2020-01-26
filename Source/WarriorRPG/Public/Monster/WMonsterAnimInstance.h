// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "Animation/AnimInstance.h"
#include "WMonsterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHitAttackDelegate);

/**
 * 
 */
UCLASS()
class WARRIORRPG_API UWMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UWMonsterAnimInstance();

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *GroundAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = true))
	UAnimMontage *TakeHitMontage;

private:
	UFUNCTION()
	void AnimNotify_HitAttack();

public:
	void PlayAttackMontage();
	void PlayTakeHitMontage();
	UAnimMontage* GetGroundAttackMontage() const;
	UAnimMontage* GetTakeHitMontage() const;

	void SetIsDead(bool IsDead);

	FOnHitAttackDelegate OnHitAttack;
};
