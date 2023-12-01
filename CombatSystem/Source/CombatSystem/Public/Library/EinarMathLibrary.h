// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Helpers/HelperStructs.h"
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
	UFUNCTION(BlueprintCallable, Category = "Math|Raycast",meta =( WorldContext="WorldContextObject", AutoCreateRefTerm="ActorsToIgnore"))
	static bool ConeCastByChannel(const UObject* WorldContextObject, FVector Start, FVector Direction,FVector ConeNormal,float Radius,int RayCount,float VisionAngle,ETraceTypeQuery TraceTypeQuery, const TArray<AActor*>& ActorsToIgnore,FTraceDebugInfo TraceDebugInfo,TArray<FHitResult>& HitResults);

	UFUNCTION(BlueprintCallable, Category = "Math|Intersection")
	static bool SphereSphereIntersection(const FVector CenterA,const float RadiusA,const FVector CenterB,const float RadiusB);


};
