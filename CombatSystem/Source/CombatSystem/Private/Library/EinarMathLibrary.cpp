// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/EinarMathLibrary.h"

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
	return FVector::CrossProduct(VectorA, VectorB).Z < 0.f?  -Angle:Angle;
}

void UEinarMathLibrary::DirectionToVector(FVector FromVector, FVector ToVector, bool Normalize, FVector& OutVector,float& NonNormLength)
{
	FVector NonNormDirection = ToVector - FromVector;
	OutVector = Normalize ? NonNormDirection.GetSafeNormal() : NonNormDirection;
	NonNormLength = NonNormDirection.Length();
}

void UEinarMathLibrary::DirectionToActor(AActor* ThisActor, AActor* TargetActor, bool Normalize, FVector& OutVector,float& NonNormLength)
{
	if (!ThisActor || !TargetActor)
	{
		return;
	}
	DirectionToVector(ThisActor->GetActorLocation(), TargetActor->GetActorLocation(), Normalize, OutVector, NonNormLength);
}

