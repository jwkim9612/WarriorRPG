// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_TurnToTarget.h"
#include "WPlayer.h"
#include "Monster/WMonster.h"
#include "Monster/AI/WAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AWMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	auto Target = Cast<AWPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AWAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	//FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	////LookVector.Z = 0.0f;

	//// MakeFromX = 주어진 로컬X축(시선방향)으로 회전값을 구하는 함수입니다
	//// MakeFromX = XAxis 만 지정된 회전 행렬을 작성합니다.
	//FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	//ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	//
	ControllingPawn->SetActorRotation(
		FMath::RInterpTo(
			ControllingPawn->GetActorRotation(), 
			UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), Target->GetActorLocation()),
			GetWorld()->GetDeltaSeconds(),
			2.0f
		)
	);

	return EBTNodeResult::Succeeded;
}
