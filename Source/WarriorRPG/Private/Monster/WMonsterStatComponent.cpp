// Fill out your copyright notice in the Description page of Project Settings.


#include "WMonsterStatComponent.h"
#include "WGameInstance.h"
#include "WMonster.h"
#include "kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWMonsterStatComponent::UWMonsterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UWMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWMonsterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWMonsterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Monster = Cast<AWMonster>(GetOwner());
	WRPGCHECK(Monster != nullptr);

	InitMonsterData(Monster);
}

void UWMonsterStatComponent::InitMonsterData(AWMonster* Monster)
{
	UWGameInstance* WGameInstance = Cast<UWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	WRPGCHECK(WGameInstance != nullptr);

	CurrentStat = WGameInstance->GetMonsterData(Monster->GetMonsterName());

	WRPGCHECK(CurrentStat != nullptr);
	CurrentHP = CurrentStat->MaxHP;
}

void UWMonsterStatComponent::SetHP(float NewHP)
{
	WRPGCHECK(CurrentStat != nullptr);
	CurrentHP = NewHP;

	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}

void UWMonsterStatComponent::SetHPToDamage(float Damage)
{
	WRPGCHECK(CurrentStat != nullptr);
	SetHP(FMath::Clamp(CurrentHP - Damage, 0.0f, CurrentStat->MaxHP));
}

float UWMonsterStatComponent::GetHP() const
{
	return CurrentHP;
}

int32 UWMonsterStatComponent::GetAttack() const
{
	return CurrentStat->Attack;
}

float UWMonsterStatComponent::GetDropExp() const
{
	return CurrentStat->DropExp;
}

