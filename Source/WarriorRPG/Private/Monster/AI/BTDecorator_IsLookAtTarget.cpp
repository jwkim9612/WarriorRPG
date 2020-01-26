// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsLookAtTarget.h"
#include "Monster/WMonster.h"
#include "Monster/AI/WAIController.h"
#include "Player/WPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_IsLookAtTarget::UBTDecorator_IsLookAtTarget()
{
	NodeName = TEXT("LookAtTarget");
}

bool UBTDecorator_IsLookAtTarget::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AWPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AWAIController::TargetKey));
	if (nullptr == Target)
		return false;

	// 타겟이 점프하면서 가까이 있을경우 폰의 Z값도 변하기 때문에 폰이 위를 쳐다보게 된다.
	// 그래서 Z값을 0으로 만든 벡터를 만들었다.
	FVector Pawn2DLocation = FVector(ControllingPawn->GetActorLocation().X, ControllingPawn->GetActorLocation().Y, 0.0f);
	FVector Target2DLocation = FVector(Target->GetActorLocation().X, Target->GetActorLocation().Y, 0.0f);

	ControllingPawn->SetActorRotation(
		FMath::RInterpTo(
			ControllingPawn->GetActorRotation(), 
			UKismetMathLibrary::FindLookAtRotation(Pawn2DLocation, Target2DLocation),
			GetWorld()->GetDeltaSeconds(), 
			5.0f
		)
	);
	
	return IsLookAtTarget(ControllingPawn, Target);
}

bool UBTDecorator_IsLookAtTarget::IsLookAtTarget(AActor* ControllingPawn,AActor* Target) const
{
	FRotator LookAtPlayerRotation = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorLocation(), Target->GetActorLocation());
	
	int32 YawOfControllingPawn = static_cast<int32>(ControllingPawn->GetActorRotation().Yaw);
	int32 YawOfLookAtTarget = static_cast<int32>(LookAtPlayerRotation.Yaw);

	if (YawOfControllingPawn <= (YawOfLookAtTarget + 5) && YawOfControllingPawn >= (YawOfLookAtTarget - 5))
		return true;
	else 
		return false;
}
