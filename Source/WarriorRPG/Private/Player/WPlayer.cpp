// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayer.h"
#include "WPlayerAnimInstance.h"
#include "WPlayerController.h"
#include "WPlayerState.h"
#include "Sound/SoundCue.h"

// Sets default values
AWPlayer::AWPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���� ���۽� 3���� ��ǥ�谡 �𸮾� 3���� ��ǥ��� �ٸ��� ������ Z������ -90�� ȸ����������Ѵ�
	// �� ������ ���� ��ġ�� �ٸ��� ������ Z������ ���� ���̸�ŭ ������� �Ѵ�.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	SpringArm			= CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera				= CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	IsAttacking = false;
	EndComboState();

	IsEmoting = false;
	bFocus = false;

	ArmLengthSpeed = 1.0f;
	PressKey = Pressed::Press_None;
}

// Called when the game starts or when spawned
void AWPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackMode(AttackMode::Default);

	WPlayerState = Cast<AWPlayerState>(GetController()->PlayerState);
	WRPGCHECK(WPlayerState != nullptr);
	WPlayerState->SetPlayerLevel(1);
}

// Called every frame
void AWPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���Ͱ� �ٶ󺸴� ���� Yaw��
	YawOfLookingDirection = GetCapsuleComponent()->GetRelativeTransform().Rotator().Yaw;

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentAttackmode)
	{
		case AttackMode::Default:
		{
			// Default���� SpringArm�� ȸ������ ������� �ʰ� ��Ʈ�ѷ��� ȸ������ ������̰�, Focus��忡���� SpringArm�� ȸ������ �̿��Ѵ�.
			// Focus���� �������� �� Focus�������� �ε巴�� ȭ���� ��ȯ��Ű�� ���� Default����� ���� ȸ������ SpringArmȸ������ �����صд�.
			SpringArm->SetRelativeRotation(GetControlRotation());
			break;
		}
		case AttackMode::Focus:
		{
			// ī�޶��� �������� ĳ���͸� �ε巴�� ȸ��
			GetCapsuleComponent()->SetRelativeRotation(FMath::RInterpTo(GetActorRotation(), FRotator(0.0f, GetControlRotation().Yaw, 0.0f), DeltaTime, /*IterpSpeed =*/20.0f));

			// �� Default���� �����ص� ȸ�������� Focus�������� ȭ���� �ε巴�� ��ȯ
			SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeTransform().Rotator(), FocusingRotation, DeltaTime, 5.0f));
			break;
		}
	}
}

void AWPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UWPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	WRPGCHECK(AnimInstance != nullptr);

	AnimInstance->OnMontageEnded.AddDynamic(this, &AWPlayer::OnAttackMontageEnded);
	AnimInstance->OnCanNextAttack.AddLambda([this]() -> void {

		if (OnComboInput)
		{
			OnComboInput = false;
			StartComboState();
			AnimInstance->JumpToNextAttackSection(CurrentCombo);
			
			WRPGCHECK(AttackSoundCue != nullptr);
			PlaySound(AttackSoundCue);
		}
	});

	AnimInstance->OnHitAttack.AddUObject(this, &AWPlayer::AttackCheck);
}

float AWPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	PlayParticle(HitParticle);
	WPlayerState->SetHPToDamage(Damage);


	return FinalDamage;
}

// Called to bind functionality to input
void AWPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���Լ��� ��� �˻��մϴ�.
	PlayerInputComponent->BindAxis("UpDown", this, &AWPlayer::UpDown);
	PlayerInputComponent->BindAxis("LeftRight", this, &AWPlayer::LeftRight);
	PlayerInputComponent->BindAxis("Turn", this, &AWPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AWPlayer::LookUp);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AWPlayer::Jump);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AWPlayer::Attack);
	PlayerInputComponent->BindAction("Focus", EInputEvent::IE_Pressed, this, &AWPlayer::OnFocus);
	PlayerInputComponent->BindAction("Focus", EInputEvent::IE_Released, this, &AWPlayer::OffFocus);
	PlayerInputComponent->BindAction("Emote1", EInputEvent::IE_Pressed, this, &AWPlayer::OnEmote1);
	PlayerInputComponent->BindAction("Emote2", EInputEvent::IE_Pressed, this, &AWPlayer::OnEmote2);
	PlayerInputComponent->BindAction("Emote3", EInputEvent::IE_Pressed, this, &AWPlayer::OnEmote3);
	PlayerInputComponent->BindAction("Skill", EInputEvent::IE_Pressed, this, &AWPlayer::Skill);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AWPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AWPlayer::OffRun);
}

void AWPlayer::UpDown(float NewAxisValue)
{
	SetForwardBackwardPressedByValue(NewAxisValue);

	if (!IsAttacking)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		//switch (CurrentAttackmode)
		//{
		//case AttackMode::Default:
		//	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		//	break;
		//case AttackMode::Focus:
		//	AddMovementInput(FRotationMatrix(FRotator(0.0f, YawOfLookingDirection, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		//	break;
		//}
	}

	// �� �Լ��� ��� �˻��ϱ� ������ �������� ���Դ����� ���� ���ϴ� �Լ� �ۼ�.
	if (NewAxisValue != 0)
	{
		if (IsEmoting)
			StopEmote();
	}
}

void AWPlayer::LeftRight(float NewAxisValue)
{
	SetLeftRightPressedByValue(NewAxisValue);

	if (!IsAttacking)
	{
		switch (CurrentAttackmode)
		{
		case AttackMode::Default:
			AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
			break;

		case AttackMode::Focus:
			AddMovementInput(FRotationMatrix(FRotator(0.0f, YawOfLookingDirection, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
			break;
		}
	}

	if (NewAxisValue != 0)
	{
		if (IsEmoting)
			StopEmote();
	}
}

void AWPlayer::Turn(float NewAxisValue)
{
	switch (CurrentAttackmode)
	{
	case AttackMode::Default:
		AddControllerYawInput(NewAxisValue);
		break;
	case AttackMode::Focus:
		if (IsAttacking)
			return;

		AddControllerYawInput(NewAxisValue / 4);
		break;
	}
}

void AWPlayer::LookUp(float NewAxisValue)
{

	switch (CurrentAttackmode)
	{
	case AttackMode::Default:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void AWPlayer::Jump()
{
	if (IsAttacking || GetMovementComponent()->IsFalling())
	{
		return;
	}
	else
	{
		Super::Jump();

		WRPGCHECK(JumpSoundCue != nullptr);
		PlaySound(JumpSoundCue);
	}
}

void AWPlayer::SetForwardBackwardPressedByValue(float AxisValue)
{
	if (AxisValue == 0)
	{
		PressKey = Pressed::Press_None;
	}
	else if (AxisValue > 0)
	{
		PressKey = Pressed::Press_Up;
	}
	else if (AxisValue < 0)
	{
		PressKey = Pressed::Press_Down;
	}
}

void AWPlayer::SetLeftRightPressedByValue(float AxisValue)
{
	if (AxisValue > 0)
	{
		PressKey = Pressed::Press_Right;
	}
	else if (AxisValue < 0)
	{
		PressKey = Pressed::Press_Left;
	}
}

void AWPlayer::Attack()
{
	if (IsEmoting)
	{
		StopEmote();
		return;
	}

	if (!IsAttacking)
	{
		IsAttacking = true;

		if (GetMovementComponent()->IsFalling())
		{
			AnimInstance->PlayAirAttackMontage();
			WRPGCHECK(AttackSoundCue != nullptr);
			PlaySound(AttackSoundCue);
			return;
		}

		StartComboState();
		AnimInstance->PlayGroundAttackMontage();

		WRPGCHECK(AttackSoundCue != nullptr);
		PlaySound(AttackSoundCue);
	}
	else
	{
		if (CanNextAttack)
		{
			OnComboInput = true;
		}
	}
}

void AWPlayer::AttackCheck()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Param(NAME_None, false, this);

	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation() + GetActorForwardVector() * 68.0f,
		GetActorLocation() + GetActorForwardVector() * 100.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(70.0f),
		Param
	);

	if (bResult)
	{
		for (auto& HitResult : HitResults)
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(WPlayerState->GetAttack() , DamageEvent, GetController(), this);
		}
	}

}

void AWPlayer::StartComboState()
{
	WRPGCHECK(MaxCombo != 0);
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, MaxCombo);
	CanNextAttack = true;
}

void AWPlayer::EndComboState()
{
	OnComboInput = false;
	CanNextAttack = false;
	CurrentCombo = 0;
}

void AWPlayer::Skill()
{
	if (!IsAttacking)
	{
		IsAttacking = true;
		AnimInstance->PlaySkillMontage();

		WRPGCHECK(SkillSoundCue != nullptr);
		PlaySound(SkillSoundCue);
	}
}

void AWPlayer::OnFocus()
{
	if (IsAttacking)
	{
		return;
	}

	SetAttackMode(AttackMode::Focus);
	bFocus = true;
}

void AWPlayer::OffFocus()
{
	if (CurrentAttackmode == AttackMode::Default)
	{
		return;
	}

	SetAttackMode(AttackMode::Default);
	bFocus = false;
}

void AWPlayer::OnRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;
}

void AWPlayer::OffRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AWPlayer::SetAttackMode(AttackMode NewAttackMode)
{
	CurrentAttackmode = NewAttackMode;

	switch (NewAttackMode)
	{
	case AttackMode::Default:
		ArmLengthTo = 600.0f;
		bUseControllerRotationYaw = false;	// ���콺�� ���� ĳ���Ͱ� �����ϰ�����.
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bDoCollisionTest = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		GetController()->SetControlRotation(FRotator(-30.0f, YawOfLookingDirection, 0.0f));
		break;

	case AttackMode::Focus:
		ArmLengthTo = 450.0f;
		bUseControllerRotationYaw = false;
		//SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = false;
		SpringArm->bDoCollisionTest = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;
	}
}

void AWPlayer::PlayParticle(UParticleSystem * ParticleSystem) const
{
	UGameplayStatics::SpawnEmitterAttached(ParticleSystem, RootComponent, NAME_None);
}

void AWPlayer::OnEmote1()
{
	if (IsAttacking)
	{ 
		return;
	}

	StartEmote(/* EmoteNum =*/ 1);
}

void AWPlayer::OnEmote2()
{
	if (IsAttacking)
	{
		return;
	}

	StartEmote(/* EmoteNum =*/ 2);
}

void AWPlayer::OnEmote3()
{
	if (IsAttacking)
	{
		return;
	}

	StartEmote(/* EmoteNum =*/ 3);
}

void AWPlayer::StartEmote(int32 EmoteNum)
{
	WRPGCHECK(EmoteNum <= 3);

	if (GetMovementComponent()->IsFalling())
	{
		return;
	}

	AnimInstance->PlayEmote(EmoteNum);
	IsEmoting = true;
}

void AWPlayer::StopEmote()
{
	WRPGCHECK(IsEmoting);

	AnimInstance->StopMontage();
	IsEmoting = false;
}

void AWPlayer::PlaySound(USoundCue * SoundToPlay)
{
	UGameplayStatics::SpawnSoundAttached(SoundToPlay, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
}

void AWPlayer::OnAttackMontageEnded(UAnimMontage * AnimMontage, bool Interrupted)
{
	if(!IsAttacking)
	{
		return;
	}

	IsAttacking = false;
	EndComboState();
}

Pressed AWPlayer::GetPressedValue() const
{
	return PressKey;
}

bool AWPlayer::IsFocusing() const
{
	return bFocus;
}