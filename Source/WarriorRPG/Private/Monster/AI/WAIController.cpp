// Fill out your copyright notice in the Description page of Project Settings.


#include "WAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/CrowdFollowingComponent.h"

const FName AWAIController::HomePosKey(TEXT("HomePos"));
const FName AWAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AWAIController::TargetKey(TEXT("Target"));

// AI에 DetourCrowd를 활성화하는 가장 쉬운 방법은 AI가 AIController의 PathFollowingComponent에 UCrowdFollowingComponent 를 사용 하도록 하는 것이다.
AWAIController::AWAIController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("BehaviorTree'/Game/Book/Monster/AI/Monster_BehaviorTree.Monster_BehaviorTree'"));

	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBObject(TEXT("BlackboardData'/Game/Book/Monster/AI/Monster_Blackboard.Monster_Blackboard'"));

	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

}

void AWAIController::BeginPlay()
{
	Super::BeginPlay();


}

void AWAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AWAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	MonsterPossessLocation = GetPawn()->GetActorLocation();
}

void AWAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, MonsterPossessLocation);
		if (!RunBehaviorTree(BTAsset))
		{
			WRPGLOG(Warning, TEXT("Not Run BehaviorTree!"));
		}
	}
}

void AWAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent != nullptr)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}