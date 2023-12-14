// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_ApplyMotion.h"

#include "CombatSystem/Abilities/CombatAbility.h"
#include "Kismet/KismetMathLibrary.h"

UCombatSystem_ApplyMotion::UCombatSystem_ApplyMotion(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bTickingTask = true;
}
UCombatSystem_ApplyMotion* UCombatSystem_ApplyMotion::CreateApplyMotionTask(UCombatAbility* OwningAbility, const FName TaskInstanceName, const FVector MotionDirection, const float MotionSpeed, const bool ControlManually, AActor* ActorToMove)
{
	
	UCombatSystem_ApplyMotion* MyObj =  NewTask<UCombatSystem_ApplyMotion>(OwningAbility,TaskInstanceName);

	MyObj->ActorToMove = ActorToMove ==nullptr? OwningAbility->GetCurrentActorInfo()->AvatarActor :ActorToMove;
	MyObj->Ability = OwningAbility;
	MyObj->MotionDirection = MotionDirection;
	MyObj->MotionSpeed = MotionSpeed;
	MyObj->ControlManually = ControlManually;
	return MyObj;
}


void UCombatSystem_ApplyMotion::Activate()
{
	if (!Ability || !ActorToMove.Get())
	{
		EndTask();
	}

}

void UCombatSystem_ApplyMotion::TickTask(float DeltaTime)
{
	const FVector NewLocation = ActorToMove->GetActorLocation() + MotionDirection * MotionSpeed * DeltaTime;
	//const FVector NewLocation = FMath::VInterpTo(ActorToMove->GetActorLocation(), ActorToMove->GetActorLocation() * MotionSpeed,MotionDirection,DeltaTime,MotionSpeed);
	ActorToMove->SetActorLocation(NewLocation, true);
}

void UCombatSystem_ApplyMotion::ExternalCancel()
{
	if (Ability)
	{
		OnCancelled.Broadcast();
	}
	Super::ExternalCancel();
}

void UCombatSystem_ApplyMotion::OnDestroy(bool bInOwnerFinished)
{
	if (Ability)
	{
		OnCompleted.Broadcast();
	}
	Super::OnDestroy(bInOwnerFinished);
}

