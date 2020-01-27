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