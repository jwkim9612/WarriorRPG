// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "GameFramework/Character.h"
#include "WPlayer.generated.h"

UCLASS()
class WARRIORRPG_API AWPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);
	virtual void Jump() override;
	void SetForwardBackwardPressedByValue(float AxisValue);
	void SetLeftRightPressedByValue(float AxisValue);

	void Attack();
	void AttackCheck();
	void StartComboState();
	void EndComboState();
	void Skill();
	void OnFocus();
	void OffFocus();
	void OnRun();
	void OffRun();
	void SetAttackMode(AttackMode NewAttackMode);
	void PlayParticle(UParticleSystem* ParticleSystem) const;

	void OnEmote1();
	void OnEmote2();
	void OnEmote3();
	void StartEmote(int32 EmoteNum);
	void StopEmote();
	void PlaySound(class USoundCue* SoundToPlay);
	
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* AnimMontage, bool Interrupted);

public:
	Pressed GetPressedValue() const;
	bool IsFocusing() const;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera", Meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;

	UPROPERTY()
	class UWPlayerAnimInstance *AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Hit", Meta = (AllowPrivateAccess = true))
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Audio", Meta = (AllowPrivateAccess = true))
	class USoundCue* JumpSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Audio", Meta = (AllowPrivateAccess = true))
	class USoundCue* AttackSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Audio", Meta = (AllowPrivateAccess = true))
	class USoundCue* SkillSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Combo", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	float YawOfLookingDirection;
	float ArmLengthTo;
	float ArmLengthSpeed;

	bool bFocus;
	bool IsEmoting;
	bool IsAttacking;
	bool OnComboInput;
	bool CanNextAttack;
	int32 CurrentCombo;

	Pressed PressKey;
	AttackMode CurrentAttackmode;
	FRotator FocusingRotation = FRotator(-30.0f, 0.0f, 0.0f);

	UPROPERTY()
	class AWPlayerState* WPlayerState;
};
