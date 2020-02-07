// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(WarriorRPG, Log, All);
#define WRPGLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define WRPGLOG_S(Verbosity) UE_LOG(WarriorRPG, Verbosity, TEXT("%s"), *WRPGLOG_CALLINFO)
#define WRPGLOG(Verbosity, Format, ...) UE_LOG(WarriorRPG, Verbosity, TEXT("%s %s"), *WRPGLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define WRPGCHECK(Expr, ...) {if(!(Expr)) { WRPGLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}