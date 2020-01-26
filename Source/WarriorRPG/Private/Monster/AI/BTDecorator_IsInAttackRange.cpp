// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "WPlayer.h"
#include "Monster/AI/WAIController.h"
#include "Monster/WMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AWMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AWPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AWAIController::TargetKey));
	if (nullptr == Target)
		return false;
	
	float ControllingPawnCapsuleRadius = ControllingPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float TargetCapsuleRadius = Target->GetCapsuleComponent()->GetScaledCapsuleRadius();
	
	// GetDistanceTo�Լ��� �� ���Ͱ��� ��ġ���� ���̸� �������ش�. 
	// �׷��� ���Ϳ� �÷��̾��� ĸ��ũ�� ������ 2 ���� ���� �����ַ��ߴµ� Log�� ���� 
	// ���Ϳ� �÷��̾��� ĸ��ũ�⸦ ���� ���� ���� ���ͼ� �� ĸ��ũ�⸦ ���־���.
	float DistanceFromPawnToTarget = (Target->GetDistanceTo(ControllingPawn)) - (ControllingPawnCapsuleRadius + TargetCapsuleRadius);

	bResult = (DistanceFromPawnToTarget <= ControllingPawn->GetAttackRange());
	return bResult;
}