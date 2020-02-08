// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayer.h"
#include "WPlayerAnimInstance.h"
#include "WPlayerController.h"
#include "WPlayerState.h"
#include "WPlayerHUDWidget.h"
#include "WQuickSlots.h"
#include "WSkillTree.h"

// Sets default values
AWPlayer::AWPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터 제작시 3차원 좌표계가 언리얼 3차원 좌표계와 다르기 때문에 Z축으로 -90도 회전시켜줘야한다
	// 또 액터의 기준 위치가 다르기 때문에 Z축으로 절반 높이만큼 내려줘야 한다.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	SpringArm			= CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera				= CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		LevelUpParticle_Object(TEXT("ParticleSystem'/Game/Book/Player/Particle/P_Whirlwind_Default_Start_01.P_Whirlwind_Default_Start_01'"));

	if (LevelUpParticle_Object.Succeeded())
	{
		LevelUpParticle = LevelUpParticle_Object.Object;
	}

	IsAttacking = false;
	EndComboState();

	IsEmoting = false;
	bFocus = false;

	ArmLengthSpeed = 1.0f;
	PressKey = Pressed::Press_None;

	IsRun = false;

	InitQuick();
}

// Called when the game starts or when spawned
void AWPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackMode(AttackMode::Default);

	WPlayerState = Cast<AWPlayerState>(GetController()->PlayerState);
	WRPGCHECK(WPlayerState != nullptr);
	WPlayerState->SetPlayerLevel(1);
	WPlayerState->OnLevelUp.AddLambda([this]() -> void {

		PlayParticle(LevelUpParticle);
	});

	WPlayerController = Cast<AWPlayerController>(GetController());
	WRPGCHECK(WPlayerController != nullptr);

	if (WPlayerController->GetPlayerHUDWidget()->WQuickSlotsWidget)
	{
		WPlayerController->GetPlayerHUDWidget()->WQuickSlotsWidget->Player = this;
		WPlayerController->GetPlayerHUDWidget()->WQuickSlotsWidget->Init();
	}

	if (WPlayerController->GetSkillTreeWidget())
	{
		WPlayerController->GetSkillTreeWidget()->Player = this;
		WPlayerController->GetSkillTreeWidget()->Init();
	}
}

// Called every frame
void AWPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 액터가 바라보는 방향 Yaw값
	YawOfLookingDirection = GetCapsuleComponent()->GetRelativeTransform().Rotator().Yaw;

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentAttackmode)
	{
		case AttackMode::Default:
		{
			// Default모드는 SpringArm의 회전값을 사용하지 않고 컨트롤러의 회전값을 사용중이고, Focus모드에서는 SpringArm의 회전값을 이용한다.
			// Focus모드로 변경했을 때 Focus시점으로 부드럽게 화면을 전환시키기 위해 Default모드일 때의 회전값을 SpringArm회전값에 대입해둔다.
			SpringArm->SetRelativeRotation(GetControlRotation());
			break;
		}
		case AttackMode::Focus:
		{
			// 카메라의 정면으로 캐릭터를 부드럽게 회전
			GetCapsuleComponent()->SetRelativeRotation(FMath::RInterpTo(GetActorRotation(), FRotator(0.0f, GetControlRotation().Yaw, 0.0f), DeltaTime, /*IterpSpeed =*/20.0f));

			// 위 Default에서 대입해둔 회전값부터 Focus시점까지 화면을 부드럽게 전환
			SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeTransform().Rotator(), FocusingRotation, DeltaTime, 5.0f));
			break;
		}
	}


	// 후에 바꿔야함.
	if (IsRun && GetCharacterMovement()->Velocity.Size() > 0.0f && !GetCharacterMovement()->IsFalling())
	{
		WPlayerState->SetMPToSkill(DeltaTime);
	}
}

void AWPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WAnimInstance = Cast<UWPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	WRPGCHECK(WAnimInstance != nullptr);

	WAnimInstance->OnMontageEnded.AddDynamic(this, &AWPlayer::OnAttackMontageEnded);
	WAnimInstance->OnCanNextAttack.AddLambda([this]() -> void {

		if (OnComboInput)
		{
			OnComboInput = false;
			StartComboState();
			WAnimInstance->JumpToNextAttackSection(CurrentCombo);
		}
	});

	WAnimInstance->OnHitAttack.AddUObject(this, &AWPlayer::AttackCheck);

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

	// 축함수는 계속 검사합니다.
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
	//PlayerInputComponent->BindAction("Skill", EInputEvent::IE_Pressed, this, &AWPlayer::Skill);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AWPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AWPlayer::OffRun);
	PlayerInputComponent->BindAction("SkillTree", EInputEvent::IE_Pressed, this, &AWPlayer::OnSkillTree);
	//PlayerInputComponent->BindAction<QuickDelegate, AWPlayer, int>("Quick_1", EInputEvent::IE_Released, this, &AWPlayer::UseQuick, 1);
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

	// 축 함수는 계속 검사하기 때문에 벨류값이 들어왔는지를 통해 원하는 함수 작성.
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
		CurrentAttackType = AttackType::Default;
		IsAttacking = true;

		if (GetMovementComponent()->IsFalling())
		{
			WAnimInstance->PlayAirAttackMontage();
			return;
		}

		StartComboState();
		WAnimInstance->PlayGroundAttackMontage();
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

	float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	float HitRange = 0.0f;
	float HitDamage = 0.0f;
	float HitScale = 0.0f;

	switch (CurrentAttackType)
	{
	case AttackType::Default:
		HitRange = 100.0f;
		HitDamage = WPlayerState->GetAttack();
		HitScale = 70.0f;
		break;
	case AttackType::Skill:
		HitRange = WSkillData[0].Range;
		HitDamage = WSkillData[0].Attack;
		HitScale = WSkillData[0].Scale;
		break;
	}

	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation() + GetActorForwardVector() * CapsuleRadius * 2,
		GetActorLocation() + GetActorForwardVector() * HitRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(HitScale),
		Param
	);

	if (bResult)
	{
		for (auto& HitResult : HitResults)
		{
			WRPGLOG(Warning, TEXT("Hit"));
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(HitDamage , DamageEvent, GetController(), this);
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

void AWPlayer::Skill(int32 Index)
{
	if (!IsAttacking)
	{
		IsAttacking = true;
		CurrentAttackType = AttackType::Skill;
		//WPlayerState->SetMPToSkill(5);
		//AnimInstance->PlaySkillMontage();
		WSkillData[Index].Use(this);
	}
}

void AWPlayer::UseQuick(int32 index)
{
	// Quick[index].Action;
}

void AWPlayer::OnSkillTree()
{
	WPlayerController->OnSkillTree();
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
	if (!IsRun)
	{
		IsRun = true;
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	}
}

void AWPlayer::OffRun()
{
	if (IsRun)
	{
		IsRun = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void AWPlayer::SetAttackMode(AttackMode NewAttackMode)
{
	CurrentAttackmode = NewAttackMode;

	switch (NewAttackMode)
	{
	case AttackMode::Default:
		ArmLengthTo = 600.0f;
		bUseControllerRotationYaw = false;	// 마우스에 따라 캐릭터가 움직일것인지.
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

	WAnimInstance->PlayEmote(EmoteNum);
	IsEmoting = true;
}

void AWPlayer::StopEmote()
{
	WRPGCHECK(IsEmoting);

	WAnimInstance->StopMontage();
	IsEmoting = false;
}

void AWPlayer::InitQuick()
{
	QuickDatas.Init(FSlotData(), 8);
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

UWPlayerAnimInstance * AWPlayer::GetAnimInstance() const
{
	return WAnimInstance;
}

AWPlayerState * AWPlayer::GetPlayerState() const
{
	return WPlayerState;
}
