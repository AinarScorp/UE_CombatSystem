// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/EinarGameplayLibrary.h"

ERelativeContext UEinarGameplayLibrary::GetActorRelativeContextSingle(const AActor* ThisActor, const FVector& Location)
{
	FVector LocalSpaceLocation = ThisActor->GetActorTransform().InverseTransformPosition(Location).GetSafeNormal();

	const bool ForwardIsLarger = FMath::Abs(LocalSpaceLocation.X) - FMath::Abs(LocalSpaceLocation.Y) > 0;
	if (ForwardIsLarger)
	{
		return LocalSpaceLocation.X > 0 ? ERelativeContext::InFront : ERelativeContext::Behind;
	}
	else
	{
		return LocalSpaceLocation.Y > 0 ? ERelativeContext::ToTheRight : ERelativeContext::ToTheLeft;
	}
}

ERelativeContext_BP UEinarGameplayLibrary::GetActorRelativeContext_BP(const AActor* Target, const FVector& Location)
{
	FVector LocalSpaceLocation = Target->GetActorTransform().InverseTransformPosition(Location).GetSafeNormal();

	const bool ForwardIsLarger = FMath::Abs(LocalSpaceLocation.X) - FMath::Abs(LocalSpaceLocation.Y) > 0;
	if (ForwardIsLarger)
	{
		return LocalSpaceLocation.X > 0 ? ERelativeContext_BP::InFront : ERelativeContext_BP::Behind;
	}
	else
	{
		return LocalSpaceLocation.Y > 0 ? ERelativeContext_BP::ToTheRight : ERelativeContext_BP::ToTheLeft;
	}
}

int32 UEinarGameplayLibrary::GetRelativeContext(const AActor* ThisActor, const AActor* Other)
{
	int32 ReturnContext = 0;

	const UE::Math::TVector<double> LocalSpaceLocation = ThisActor->GetActorTransform().InverseTransformPosition(Other->GetActorLocation());

	if (LocalSpaceLocation.X > 0)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::InFront);
	}
	else if (LocalSpaceLocation.X < 0)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::Behind);
	}

	if (LocalSpaceLocation.Y > 0)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::ToTheRight);
	}
	else if (LocalSpaceLocation.Y < 0)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::ToTheLeft);
	}

	if (LocalSpaceLocation.Z > 0)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::Above);
	}
	else if (LocalSpaceLocation.Z < 0)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::Below);
	}
	return ReturnContext;
}

bool UEinarGameplayLibrary::ContextPredicate(const int32 Test, const int32 Value)
{	
	return (Test & Value) == Value;
}
