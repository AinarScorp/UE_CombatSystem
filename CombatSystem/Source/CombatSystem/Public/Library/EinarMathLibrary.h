// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EinarMathLibrary.generated.h"


/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UEinarMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Angle")
	static float GetAngleBetweenVectors(FVector VectorA,FVector VectorB, bool GetInDeg = true, bool GetFullCircle = false);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Vector")
	static void DirectionToVector(FVector FromVector, FVector ToVector, bool Normalize, FVector& OutVector, float& NonNormLength);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Vector")
	static void DirectionToActor(AActor* ThisActor, AActor* TargetActor, bool Normalize, FVector& OutVector, float& NonNormLength);

	
};
