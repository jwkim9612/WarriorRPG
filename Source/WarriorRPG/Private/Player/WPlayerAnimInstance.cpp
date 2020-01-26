// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerAnimInstance.h"
#include "WPlayer.h"

UWPlayerAnimInstance::UWPlayerAnimInstance()
{

}

void UWPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto PlayerCharacter = Cast<AWPlayer>(TryGetPawnOwner());
	if (PlayerCharacter != nullptr)
	{
		CurrentSpeed	= PlayerCharacter->GetVelocity().Size();
		IsInAir			= PlayerCharacter->GetMovementComponent()->IsFalling();
		IsFocusMode		= PlayerCharacter->IsFocusing();
		PressKey		= PlayerCharacter->GetPressedValue();
	}
}

void UWPlayerAnimInstance::AnimNotify_CanNextAttack()
{
	OnCanNextAttack.Broadcast();
}

void UWPlayerAnimInstance::AnimNotify_HitAttack()
{
	OnHitAttack.Broadcast();
}

void UWPlayerAnimInstance::PlayGroundAttackMontage()
{
	Montage_Play(GroundAttackMontage, 1.0f);
}

void UWPlayerAnimInstance::PlayAirAttackMontage()
{
	Montage_Play(AirAttackMontage, 1.0f);
}

void UWPlayerAnimInstance::PlaySkillMontage()
{
	Montage_Play(SkillMontage, 1.0f);
}

void UWPlayerAnimInstance::JumpToNextAttackSection(int32 NewSection)
{
	WRPGCHECK(Montage_IsPlaying(GroundAttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), GroundAttackMontage);
}

FName UWPlayerAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

void UWPlayerAnimInstance::PlayEmote(int32 EmoteNum)
{
	switch (EmoteNum)
	{
	case 1:
		WRPGCHECK(Emote1Montage);
		Montage_Play(Emote1Montage, 1.0f);
		break;
	case 2:
		WRPGCHECK(Emote2Montage);
		Montage_Play(Emote2Montage, 1.0f);
		break;
	case 3:
		WRPGCHECK(Emote3Montage);
		Montage_Play(Emote3Montage, 1.0f);
		break;
	}
}

void UWPlayerAnimInstance::StopMontage()
{
	Montage_Stop(0.1f);
}
