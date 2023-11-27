// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/EinarGameplayLibrary.h"

ERelativeContext UEinarGameplayLibrary::GetActorRelativeContext(const AActor* Target, const FVector& Location)
{
	FVector LocalSpaceLocation = Target->GetActorTransform().InverseTransformPosition(Location);

	const bool ForwardIsLarger = FMath::Abs(LocalSpaceLocation.X) - FMath::Abs(LocalSpaceLocation.Y) > 0;
	if (ForwardIsLarger)
	{
		return LocalSpaceLocation.X>0? ERelativeContext::InFront: ERelativeContext::Behind;
	}
	else
	{
		return LocalSpaceLocation.Y>0? ERelativeContext::ToTheRight: ERelativeContext::ToTheLeft;
	}

}
