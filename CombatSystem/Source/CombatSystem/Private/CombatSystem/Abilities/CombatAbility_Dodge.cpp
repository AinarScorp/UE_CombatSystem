// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/CombatAbility_Dodge.h"

#include "Library/EinarGameplayLibrary.h"
#include "Player/Controller/CombatSystem_PlayerController.h"

void UCombatAbility_Dodge::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, TriggerEventData);
	FMontageWithSection* DodgeMontage = GetDodgeMontage();
	if (!DodgeMontage)
	{
		InternalCancelAbility();
		return;
	}
	
	UCombatSystem_PlayMontage *MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, GetFName(), DodgeMontage->AnimMontage, 1, DodgeMontage->AnimSection);
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbility_Dodge::InternalEndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UCombatAbility_Dodge::InternalCancelAbility);
	MontageTask->ReadyForActivation();
}

FMontageWithSection* UCombatAbility_Dodge::GetDodgeMontage()
{
	//if Locking onto the enemy
	//return GetDodgeMontageFromLocking();
	//else
	return RotateToMoveInput()? GetDodgeMontage(ERelativeContext::InFront) : GetDodgeMontage(ERelativeContext::Behind);
}

FMontageWithSection* UCombatAbility_Dodge::GetDodgeMontageFromLocking()
{
	const ACombatSystem_PlayerController* PlayerController = Cast<ACombatSystem_PlayerController>(CurrentActorInfo->Controller);
	const AActor* Avatar = CurrentActorInfo->AvatarActor.Get();

	const FVector2D MoveInput =PlayerController->GetMoveInput();

	const FVector ControllerForward = Avatar->GetActorForwardVector() *(MoveInput.X);
	const FVector ControllerRight = Avatar->GetActorRightVector() * (MoveInput.Y);

	const FVector RelativeMoveInput = Avatar->GetActorLocation()+ ControllerForward+ControllerRight;
	
	const int32 RelativeContext = UEinarGameplayLibrary::GetRelativeContextFromVector(Avatar, RelativeMoveInput);
	for (auto& DodgeMontage : DodgeMontages)
	{
		if (DodgeMontage.Direction ==RelativeContext )
		{
			return &DodgeMontage.Montage;
		}
	}
	return nullptr;
}

FMontageWithSection* UCombatAbility_Dodge::GetDodgeMontage(ERelativeContext RelativeContext)
{
	for (FDodgeMontage& DodgeMontage : DodgeMontages)
	{
		if (UEinarGameplayLibrary::FlagPredicate(DodgeMontage.Direction,RelativeContext))
		{
			return &DodgeMontage.Montage;
		}
	}
	return nullptr;
}



bool UCombatAbility_Dodge::RotateToMoveInput() const
{
	if (!CurrentActorInfo->Controller.Get()) return false;
	const ACombatSystem_PlayerController* PlayerController = Cast<ACombatSystem_PlayerController>(CurrentActorInfo->Controller);
	if (!PlayerController) return false;
	return UEinarGameplayLibrary::RotateToMoveInput(CurrentActorInfo->AvatarActor.Get(),PlayerController->GetMoveInput(), PlayerController->GetControlRotation().Yaw);

}
