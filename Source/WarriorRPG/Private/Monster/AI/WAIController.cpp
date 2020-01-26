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

// AI�� DetourCrowd�� Ȱ��ȭ�ϴ� ���� ���� ����� AI�� AIController�� PathFollowingComponent�� UCrowdFollowingComponent �� ��� �ϵ��� �ϴ� ���̴�.
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




//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "WarriorAIController.h"
//#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Character/PlayerCharacter.h"
//#include "WarriorMonster.h"
//#include "Kismet/KismetMathLibrary.h"
//#include "..\..\Public\Monster\WarriorAIController.h"
//
//AWarriorAIController::AWarriorAIController()
//{
//	PrimaryActorTick.bCanEverTick = true;
//
//	//��������_�÷�
//	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHTCONFIG"));
//	// AI ������ ���� ������Ʈ.
//	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PERRCEPTIONCOMPONENT")));
//
//	SightConfig->SightRadius = AISightRadius;
//	SightConfig->LoseSightRadius = AILoseSightRadius;
//	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
//	SightConfig->SetMaxAge(AISightAge);
//
//	// ������ �����ϵ���.
//	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
//	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
//	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//
//	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
//	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AWarriorAIController::OnPawnDetected);
//	GetPerceptionComponent()->ConfigureSense(*SightConfig);
//
//	AttackTimer = 0.3f;
//}
//
//void AWarriorAIController::BeginPlay()
//{
//	Super::BeginPlay();
//
//	HomePos = GetPawn()->GetActorLocation();
//}
//
//void AWarriorAIController::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//
//}
//
//void AWarriorAIController::OnPossess(APawn * InPawn)
//{
//	Super::OnPossess(InPawn);
//
//	Monster = Cast<AWarriorMonster>(InPawn);
//	WRPGCHECK(Monster != nullptr);
//}
//
//void AWarriorAIController::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	// �����ϸ鼭 ���������� ���� �� �Ÿ��� ���� ���� ���ͼ� �ϴ� -20ó���س���.
//	if (DistanceToPlayer - 20 > AISightRadius)
//	{
//		IsPlayerDetected = false;
//	}
//
//	if (IsPlayerDetected)
//	{
//		if (Player != nullptr)
//		{
//			DistanceToPlayer = GetPawn()->GetDistanceTo(Player);
//		}
//
//		FRotator LookAtPlayerRotation = UKismetMathLibrary::FindLookAtRotation(Monster->GetActorLocation(), Player->GetActorLocation());
//		Monster->SetActorRotation(LookAtPlayerRotation);
//
//		if (IsInAttackRange())
//		{
//			IsAttacking = true;
//			Attack(/*Delay =*/0.3f);
//
//			Monster->OnAttackEnd.AddLambda([this]() -> void {
//				IsAttacking = false;
//			});
//
//			//Monster->GetCharacterMovement()->DisableMovement();
//		}
//		else
//		{
//			if (!IsAttacking)
//			{
//				MoveToActor(Player, 5.0f);
//			}
//		}
//	}
//	else
//	{
//		if (!IsAttacking)
//		{
//			MoveToLocation(HomePos);
//		}
//	}
//
//}
//
//FRotator AWarriorAIController::GetControlRotation() const
//{
//	if (GetPawn() == nullptr)
//	{
//		return FRotator::ZeroRotator;
//	}
//
//	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
//}
//
//void AWarriorAIController::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
//{
//	for (auto& DetectedPawn : DetectedPawns)
//	{
//		auto Pawn = Cast<APawn>(DetectedPawn);
//		if (Pawn->IsPlayerControlled())
//		{
//			SetPlayer(DetectedPawn);
//			DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawn);
//			IsPlayerDetected = true;
//		}
//	}
//}
//
//void AWarriorAIController::SetPlayer(AActor* NewPlayer)
//{
//	Player = Cast<APlayerCharacter>(NewPlayer);
//}
//
//bool AWarriorAIController::IsInAttackRange() const
//{
//	if (IsPlayerDetected && DistanceToPlayer < 100)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//void AWarriorAIController::Attack(float Delay)
//{
//	if (Delay == 0.0f)
//	{
//		Monster->Attack();
//	}
//	else
//	{
//		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
//			Monster->Attack();
//		}), Delay, false);
//	}
//}