// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/EinarMathLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

float UEinarMathLibrary::GetAngleBetweenVectors(FVector VectorA, FVector VectorB, const bool GetInDeg, const bool GetFullCircle)
{
	VectorA.Normalize();
	VectorB.Normalize();
	const float DotProduct = FVector::DotProduct(VectorA, VectorB);
	const float Angle = GetInDeg ? FMath::RadiansToDegrees(FMath::Acos(DotProduct)) : FMath::Acos(DotProduct);
	if (!GetFullCircle)
	{
		return Angle;
	}
	return FVector::CrossProduct(VectorA, VectorB).Z < 0.f ? -Angle : Angle;
}

void UEinarMathLibrary::DirectionToVector(FVector FromVector, FVector ToVector, bool Normalize, FVector& OutVector, float& NonNormLength)
{
	FVector NonNormDirection = ToVector - FromVector;
	OutVector = Normalize ? NonNormDirection.GetSafeNormal() : NonNormDirection;
	NonNormLength = NonNormDirection.Length();
}

void UEinarMathLibrary::DirectionToActor(AActor* ThisActor, AActor* TargetActor, bool Normalize, FVector& OutVector, float& NonNormLength)
{
	if (!ThisActor || !TargetActor)
	{
		return;
	}
	DirectionToVector(ThisActor->GetActorLocation(), TargetActor->GetActorLocation(), Normalize, OutVector, NonNormLength);
}

bool UEinarMathLibrary::ConeCastByChannel(const UObject* WorldContextObject, FVector Start, FVector Direction, FVector ConeNormal, float Radius, int RayCount, float VisionAngle, ETraceTypeQuery TraceTypeQuery, const TArray<AActor*>& ActorsToIgnore, FTraceDebugInfo TraceDebugInfo, TArray<FHitResult>& HitResults)
{
	for (float r = 0; r <= RayCount; ++r)
	{
		float Angle = (r) / float(RayCount) * VisionAngle;
		FVector End = UKismetMathLibrary::RotateAngleAxis(Direction, Angle, ConeNormal) * Radius;
		End += Start;
		//FVector End = Direction * Radius;
		FHitResult OutHitResult;
		bool SuccessfulHit = UKismetSystemLibrary::LineTraceSingle(
			WorldContextObject, Start, End, TraceTypeQuery, false,
			ActorsToIgnore, TraceDebugInfo.DrawDebugTraceType, OutHitResult, true, TraceDebugInfo.TraceColor, TraceDebugInfo.TraceHitColor, TraceDebugInfo.DrawTime);
		if (SuccessfulHit)
		{
			HitResults.Add(OutHitResult);
		}
		Angle *= -1;
		End = UKismetMathLibrary::RotateAngleAxis(Direction, Angle, ConeNormal) * Radius;
		End += Start;
		OutHitResult.Reset();
		//End = Direction * Radius;
		SuccessfulHit = UKismetSystemLibrary::LineTraceSingle(
			WorldContextObject, Start, End, TraceTypeQuery, false,
			ActorsToIgnore, TraceDebugInfo.DrawDebugTraceType, OutHitResult, true, TraceDebugInfo.TraceColor, TraceDebugInfo.TraceHitColor, TraceDebugInfo.DrawTime);
		if (SuccessfulHit)
		{
			HitResults.Add(OutHitResult);
		}
	}
	return !HitResults.IsEmpty();
}

bool UEinarMathLibrary::SphereSphereIntersection(const FVector CenterA, const float RadiusA, const FVector CenterB, const float RadiusB)
{
	const FVector Difference = CenterA - CenterB;
	const float RadiusSum = RadiusA + RadiusB;
	return Difference.Dot(Difference) <= RadiusSum * RadiusSum;
}
