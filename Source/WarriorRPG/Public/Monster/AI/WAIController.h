// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorRPG.h"
#include "AIController.h"
#include "WAIController.generated.h"

/**
 * 
 */
UCLASS()
class WARRIORRPG_API AWAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AWAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
	
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	
	void RunAI();
	void StopAI();

private:
	UPROPERTY()
	UBehaviorTree* BTAsset;

	UPROPERTY()
	UBlackboardData* BBAsset;

	FVector MonsterPossessLocation;
};


//#pragma once
//
//#include "WarriorRPG.h"
//#include "AIController.h"
//#include "WarriorAIController.generated.h"
//
///**
// *
// */
//UCLASS()
//class WARRIORRPG_API AWarriorAIController : public AAIController
//{
//	GENERATED_BODY()
//
//public:
//	AWarriorAIController();
//
//protected:
//	virtual void BeginPlay() override;
//	virtual void PostInitializeComponents() override;
//	virtual void OnPossess(APawn* InPawn) override;
//	virtual void Tick(float DeltaTime) override;
//	virtual FRotator GetControlRotation() const override;
//
//private:
//	UFUNCTION()
//		void OnPawnDetected(const TArray<AActor*> &DetectedPawns);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", Meta = (AllowPrivateAccess = true))
//		float AISightRadius = 500.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", Meta = (AllowPrivateAccess = true))
//		float AISightAge = 5.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", Meta = (AllowPrivateAccess = true))
//		float AILoseSightRadius = AISightRadius + 50.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", Meta = (AllowPrivateAccess = true))
//		float AIFieldOfView = 180.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", Meta = (AllowPrivateAccess = true))
//		class UAISenseConfig_Sight* SightConfig;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", Meta = (AllowPrivateAccess = true))
//		bool IsPlayerDetected = false;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", Meta = (AllowPrivateAccess = true))
//		float DistanceToPlayer = 0.0f;
//
//	FVector HomePos = FVector::ZeroVector;
//	bool IsAttacking = false;
//
//	class APlayerCharacter* Player;
//	class AWarriorMonster* Monster;
//
//	void SetPlayer(AActor* NewPlayer);
//	bool IsInAttackRange() const;
//
//	void Attack(float Delay = 0.0f);
//
//	UPROPERTY()
//		float AttackTimer;
//
//	FTimerHandle AttackTimerHandle = {};
//};
