// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/MoveAbility.h"

#include "InputActionValue.h"
#include "Tags/CombatSystem_GameplayTags.h"

UMoveAbility::UMoveAbility()
{
	const FCombatSystem_GameplayTags& GameplayTags = FCombatSystem_GameplayTags::Get();
	AbilityTags.AddTag(GameplayTags.InputTag_Move);
}

void UMoveAbility::OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::OnGiveAbility(Handle, ActorInfo);
	OnInputActionValuesSet.AddDynamic(this,&ThisClass::ExecuteMove);
}

void UMoveAbility::ExecuteMove(const FInputActionValue& InputActionValue)
{
	if (!bIsActive)
	{
		return;
	}
	FVector2D MoveInput = InputActionValue.Get<FVector2D>();
	
	if (MoveInput == FVector2D::ZeroVector)
	{
		InternalEndAbility();
	}
	if (!CurrentActorInfo->Controller.Get()) return;

	UE_LOG(LogTemp, Warning, TEXT("I am moving with input %s"), *MoveInput.ToString())
	const FRotator YawRotation(0.f, CurrentActorInfo->Controller->GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = CurrentActorInfo->Controller->GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MoveInput.X);
		ControlledPawn->AddMovementInput(RightDirection, MoveInput.Y);
	}
}
