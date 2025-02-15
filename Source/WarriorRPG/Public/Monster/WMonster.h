// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "GameFramework/Character.h"
#include "WMonster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class WARRIORRPG_API AWMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* AnimMontage, bool Interrupted);

	UFUNCTION()
	void OnTakaHitMontageEnded(UAnimMontage* AnimMontage, bool Interrupted);

	UFUNCTION()
	void OnVisibleHPBarBoxBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnVisibleHPBarBoxEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	class UWMonsterAnimInstance *WAnimInstance;

	UPROPERTY()
	class AWAIController *WAIController;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UWMonsterStatComponent* CurrentStat;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPWidget;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UBoxComponent* VisibleHPBarBox;

	UPROPERTY(EditDefaultsOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Name", Meta = (AllowPrivateAcctess = true))
	FName MonsterName;

	bool IsAttacking = false;
	
	FTimerHandle DeadTimerHandle;
	float DeadTimer;
	bool IsDead = false;

public:
	void Attack();
	void AttackCheck();
	void Dead();
	float GetAttackRange() const;
	FName GetMonsterName() const;
	class UWMonsterStatComponent* GetCurrentStat() const;

	FOnAttackEndDelegate OnAttackEnd;
};
