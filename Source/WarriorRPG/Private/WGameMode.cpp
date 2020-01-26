// Fill out your copyright notice in the Description page of Project Settings.


#include "WGameMode.h"
#include "WPlayerController.h"

AWGameMode::AWGameMode()
{
	//static ConstructorHelpers::FClassFinder<APawn>
	//	DefaultPlayerPawn(TEXT("Blueprint'/Game/Book/BP/Khaimera/Khaimera_Player.Khaimera_Player_C'"));

	//if (DefaultPlayerPawn.Succeeded())
	//{
	//	DefaultPawnClass = DefaultPlayerPawn.Class;
	//}

	PlayerControllerClass = AWPlayerController::StaticClass();

}