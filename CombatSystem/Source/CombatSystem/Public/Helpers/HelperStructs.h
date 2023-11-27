// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HelperStructs.generated.h"

USTRUCT(BlueprintType)
struct FTraceDebugInfo
{
	GENERATED_BODY()
public:
	FTraceDebugInfo()
	{
		DrawDebugTraceType = EDrawDebugTrace::Type::None;
		TraceColor = FLinearColor::Blue;
		TraceHitColor = FLinearColor::Green;
	}
public:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceType;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor TraceColor;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor TraceHitColor;
	UPROPERTY(EditDefaultsOnly)
	float DrawTime = 5.f;

};