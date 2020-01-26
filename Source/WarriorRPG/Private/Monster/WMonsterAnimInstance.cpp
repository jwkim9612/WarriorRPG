// Fill out your copyright notice in the Description page of Project Settings.


#include "WMonsterAnimInstance.h"
#include "WMonster.h"

UWMonsterAnimInstance::UWMonsterAnimInstance()
{

}

void UWMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Monster = Cast<AWMonster>(TryGetPawnOwner());
	if (Monster != nullptr)
	{
		CurrentSpeed = Monster->GetVelocity().Size();
	}
}

void UWMonsterAnimInstance::AnimNotify_HitAttack()
{
	OnHitAttack.Broadcast();
}

void UWMonsterAnimInstance::PlayAttackMontage()
{
	Montage_Play(GroundAttackMontage);
}

void UWMonsterAnimInstance::PlayTakeHitMontage()
{
	Montage_Play(TakeHitMontage);
}

UAnimMontage * UWMonsterAnimInstance::GetGroundAttackMontage() const
{
	return GroundAttackMontage;
}

UAnimMontage * UWMonsterAnimInstance::GetTakeHitMontage() const
{
	return TakeHitMontage;
}

void UWMonsterAnimInstance::SetIsDead(bool IsDead)
{
	this->IsDead = IsDead;
}
