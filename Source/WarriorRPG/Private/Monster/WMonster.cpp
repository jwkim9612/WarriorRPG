// Fill out your copyright notice in the Description page of Project Settings.


#include "WMonster.h"
#include "WMonsterAnimInstance.h"
#include "WAIController.h"
#include "WMonsterStatComponent.h"
#include "WPlayerController.h"

// Sets default values
AWMonster::AWMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 캐릭터 제작시 3차원 좌표계가 언리얼 3차원 좌표계와 다르기 때문에 Z축으로 -90도 회전시켜줘야한다
	// 또 액터의 기준 위치가 다르기 때문에 Z축으로 절반 높이만큼 내려줘야 한다.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));

	// AI가 부드럽게 회전하려면 Use Controller Rotation(Pawn 카테고리) 을 언체크하고 
	// Orient Rotation to Movement(Character Movement 카테고리) 를 체크한다.
	// 원리는 컨트롤러의 회전값이 아닌 캐릭터 이동 방향의 회전값을 기준으로 부드러운 회전을 하도록 하기때문
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AWAIController::StaticClass();

	CurrentStat = CreateDefaultSubobject<UWMonsterStatComponent>(TEXT("MonsterStat"));
	
	DeadTimer = 3.0f;
}

// Called when the game starts or when spawned
void AWMonster::BeginPlay()
{
	Super::BeginPlay();

	WAIController = Cast<AWAIController>(GetController());
	WAIController->RunAI();

	WRPGCHECK(AttackRange > 0.0f);
	WRPGCHECK(MonsterName != NAME_None);
}

void AWMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WAnimInstance = Cast<UWMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	WRPGCHECK(WAnimInstance != nullptr);

	WAnimInstance->OnMontageEnded.AddDynamic(this, &AWMonster::OnAttackMontageEnded);
	WAnimInstance->OnMontageEnded.AddDynamic(this, &AWMonster::OnTakaHitMontageEnded);
	WAnimInstance->OnHitAttack.AddUObject(this, &AWMonster::AttackCheck);
	CurrentStat->OnHPIsZero.AddUObject(this, &AWMonster::Dead);

	
}

float AWMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	PlayParticle(HitParticle);
	
	WAIController->StopAI();
	WAnimInstance->PlayTakeHitMontage();

	CurrentStat->SetHPToDamage(Damage);
	WRPGLOG(Warning, TEXT("%f"), CurrentStat->GetHP());
	
	if (IsDead)
	{
		AWPlayerController* WPlayerController = Cast<AWPlayerController>(EventInstigator);
		WPlayerController->MonsterKill(this);
	}

	return FinalDamage;
}

// Called every frame
void AWMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWMonster::OnAttackMontageEnded(UAnimMontage * AnimMontage, bool Interrupted)
{
	if (WAnimInstance->GetGroundAttackMontage() != AnimMontage)
		return;

	if (!IsAttacking)
	{
		return;
	}

	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AWMonster::OnTakaHitMontageEnded(UAnimMontage * AnimMontage, bool Interrupted)
{
	if (IsDead)
		return;

	if (WAnimInstance->GetTakeHitMontage() != AnimMontage)
		return;

	WAIController->RunAI();
}

void AWMonster::PlayParticle(UParticleSystem * ParticleSystem)
{
	UGameplayStatics::SpawnEmitterAttached(ParticleSystem, RootComponent, NAME_None);
}

void AWMonster::Attack()
{
	if (!IsAttacking)
	{
		IsAttacking = true;
		WAnimInstance->PlayAttackMontage();
	}
}

void AWMonster::AttackCheck()
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Param(NAME_None, false, this);

	// 공격 시작위치벡터는 액터의 위치백터 + ( 액터의 forward벡터 * ( 엑터의 캡슐크기 / 2 ) );
	const FVector AttackStartLocation = GetActorLocation() + GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() / 2);
	const FVector AttackEndLocation = GetActorLocation() + GetActorForwardVector() * ((GetCapsuleComponent()->GetScaledCapsuleRadius() / 2) + AttackRange);

	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		AttackStartLocation,
		AttackEndLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(GetCapsuleComponent()->GetScaledCapsuleRadius()),
		Param
	);

	if (bResult)
	{
		for (auto& HitResult : HitResults)
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CurrentStat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}

void AWMonster::Dead()
{
	IsDead = true;
	WAnimInstance->SetIsDead(true);
	WAIController->StopAI();
	SetActorEnableCollision(false);

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		Destroy();
	}), DeadTimer, false);
}

float AWMonster::GetAttackRange() const
{
	return AttackRange;
}

FName AWMonster::GetMonsterName() const
{
	return MonsterName;
}

UWMonsterStatComponent * AWMonster::GetCurrentStat() const
{
	return CurrentStat;
}
