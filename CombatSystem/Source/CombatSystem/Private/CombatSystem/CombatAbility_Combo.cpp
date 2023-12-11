// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/CombatAbility_Combo.h"

#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"
#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/Controller/CombatSystem_PlayerController.h"
#include "Tags/CombatSystem_GameplayTags.h"

void UCombatAbility_Combo::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo,TriggerEventData);
	CurrentComboIndex = 0;
	ListeningToInput = false;
	StartNextAttack();
}

void UCombatAbility_Combo::InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	if (!ListeningToInput) return;

	ListeningToInput = false;
	Super::InputPressed(Handle, ActorInfo);
	StartNewWaitTaskForInputWindowEnd();

}

void UCombatAbility_Combo::EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled)
{
	if (WaitForInputWindowStartTask.IsValid())
	{
		WaitForInputWindowStartTask->ExternalCancel();
	}
	if (WaitForInputWindowEndTask.IsValid())
	{
		WaitForInputWindowEndTask->ExternalCancel();
	}
	if (MontageTask.IsValid())
	{
		MontageTask->ExternalCancel();
	}
	Super::EndAbility(Handle, ActorInfo, bWasCancelled);
}

void UCombatAbility_Combo::StartNextAttack()
{
	if (Combos.Num()<= CurrentComboIndex) return;
	const FComboAnimInfo& ComboInfo = Combos[CurrentComboIndex];
	CurrentComboIndex++;
	RotateToMoveInput();
	MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, "AttackCombo", ComboInfo.AnimMontage, 1, ComboInfo.AnimSection);
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbility_Combo::InternalEndAbility);
	MontageTask->ReadyForActivation();
	
	StartNewWaitTaskForInputWindowStart();
}
void UCombatAbility_Combo::StartNewWaitTaskForInputWindowStart()
{
	if (WaitForInputWindowStartTask.IsValid())
	{
		WaitForInputWindowStartTask->ExternalCancel();
	}
	WaitForInputWindowStartTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(CurrentActorInfo->AvatarActor.Get(),FCombatSystem_GameplayTags::Get().AttackInputWindow_Start,true);
	WaitForInputWindowStartTask->EventReceived.AddDynamic(this, &UCombatAbility_Combo::AttackInputWindowStarted);
	WaitForInputWindowStartTask->ReadyForActivation();
}

void UCombatAbility_Combo::StartNewWaitTaskForInputWindowEnd()
{
	if (WaitForInputWindowEndTask.IsValid())
	{
		WaitForInputWindowStartTask->EndTask();
	}
	WaitForInputWindowEndTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(CurrentActorInfo->AvatarActor.Get(), FCombatSystem_GameplayTags::Get().AttackInputWindow_End,true);
	WaitForInputWindowEndTask->EventReceived.AddDynamic(this, &UCombatAbility_Combo::AttackInputWindowEnded);
	WaitForInputWindowEndTask->ReadyForActivation();
}


void UCombatAbility_Combo::AttackInputWindowStarted(FCombatEventData Payload)
{
	ListeningToInput = true;
}

void UCombatAbility_Combo::AttackInputWindowEnded(FCombatEventData Payload)
{
	StartNextAttack();
}

void UCombatAbility_Combo::RotateToMoveInput() const
{
	if (!bShouldRotateToInput || !CurrentActorInfo->Controller.Get()) return;
	const ACombatSystem_PlayerController* PlayerController = Cast<ACombatSystem_PlayerController>(CurrentActorInfo->Controller);
	if (!PlayerController) return;

	const FVector2D MoveInput =PlayerController->GetMoveInput();
	if (MoveInput == FVector2D::ZeroVector) return;
	const FRotator RotFromMoveInput = UKismetMathLibrary::MakeRotFromX(FVector(MoveInput.X, MoveInput.Y,0));
	const FRotator RotFromCurrentRotation = FRotator(0, CurrentActorInfo->Controller->GetControlRotation().Yaw,0);
	
	CurrentActorInfo->AvatarActor->SetActorRotation(UKismetMathLibrary::ComposeRotators(RotFromMoveInput, RotFromCurrentRotation));
}



