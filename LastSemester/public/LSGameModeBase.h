// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LSGameModeBase.generated.h"


#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINT_CALLINFO() UE_LOG(LogTemp, Warning, TEXT("%s"), *CALLINFO)
#define PRINT_LOG(fmt, ...) UE_LOG(LogTemp, Warning, TEXT("%s: ") fmt, *CALLINFO, ##__VA_ARGS__)

/**
 * 
 */
UCLASS()
class LASTSEMESTER_API ALSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALSGameModeBase();

};
