// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/EinarGameplayLibrary.h"

#include "Kismet/KismetMathLibrary.h"

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
	return GetRelativeContextFromVector(ThisActor,Other->GetActorLocation());
}

int32 UEinarGameplayLibrary::GetRelativeContextFromVector(const AActor* ThisActor, const FVector RelativeVector)
{
	int32 ReturnContext = 0;

	const UE::Math::TVector<double> LocalSpaceLocation = ThisActor->GetActorTransform().InverseTransformPosition(RelativeVector);
	UE_LOG(LogTemp,Display,TEXT("LocalSpaceLocation equals: %s"),*LocalSpaceLocation.ToString())
	if (LocalSpaceLocation.X > 0.00001f)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::InFront);
	}
	else if (LocalSpaceLocation.X < -0.00001f)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::Behind);
	}

	if (LocalSpaceLocation.Y > 0.00001f)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::ToTheRight);
	}
	else if (LocalSpaceLocation.Y < -0.00001f)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::ToTheLeft);
	}

	if (LocalSpaceLocation.Z > 0.00001f)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::Above);
	}
	else if (LocalSpaceLocation.Z < -0.00001f)
	{
		ReturnContext |= 1 << static_cast<int32>(ERelativeContext::Below);
	}
	return ReturnContext;
}

bool UEinarGameplayLibrary::ContextPredicate(const int32 Test, const int32 Value)
{	
	return (Test & Value) == Value;
}

bool UEinarGameplayLibrary::FlagPredicate(const int32 Test, const int32 Value)
{
	return (((Test) & (1 << static_cast<int32>(Value))) > 0);
}

bool UEinarGameplayLibrary::RotateToMoveInput(AActor* ActorToRotate, const FVector2D MoveInput, const float CurrentYaw)
{
	if (MoveInput == FVector2D::ZeroVector) return false;
	const FRotator RotFromMoveInput = UKismetMathLibrary::MakeRotFromX(FVector(MoveInput.X, MoveInput.Y,0));
	const FRotator CurrentRotation = FRotator(0, CurrentYaw,0);
	ActorToRotate->SetActorRotation(UKismetMathLibrary::ComposeRotators(RotFromMoveInput, CurrentRotation));
	return true;
}
