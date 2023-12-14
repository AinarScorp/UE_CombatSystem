// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/CombatAbility_Combo.h"

#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"
#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/EinarGameplayLibrary.h"
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

	// if (WaitForInputWindowStartTask.IsValid())
	// {
	// 	WaitForInputWindowStartTask->ExternalCancel();
	// }
	// if (WaitForInputWindowEndTask.IsValid())
	// {
	// 	WaitForInputWindowEndTask->ExternalCancel();
	// }
	// if (MontageTask.IsValid())
	// {
	// 	MontageTask->ExternalCancel();
	// }
	Super::EndAbility(Handle, ActorInfo, bWasCancelled);
}

void UCombatAbility_Combo::StartNextAttack()
{
	if (Combos.Num()<= CurrentComboIndex) return;
	const FComboAnimInfo& ComboInfo = Combos[CurrentComboIndex];
	CurrentComboIndex++;
	RotateToMoveInput();

	if (MontageTask.Get())
	{
		MontageTask->OnInterrupted.RemoveDynamic(this, &UCombatAbility_Combo::InternalCancelAbility);
	}
	MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, GetFName(), ComboInfo.AnimMontage, 1, ComboInfo.AnimSection);
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbility_Combo::InternalEndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UCombatAbility_Combo::InternalCancelAbility);
	MontageTask->ReadyForActivation();
	StartNewWaitTaskForInputWindowStart();
}
void UCombatAbility_Combo::StartNewWaitTaskForInputWindowStart()
{
	if (WaitForInputWindowStartTask.IsValid())
	{
		WaitForInputWindowStartTask->ExternalCancel();
	}
	WaitForInputWindowStartTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this,FCombatSystem_GameplayTags::Get().AttackInputWindow_Start,true);
	WaitForInputWindowStartTask->EventReceived.AddDynamic(this, &UCombatAbility_Combo::AttackInputWindowStarted);
	WaitForInputWindowStartTask->ReadyForActivation();
}

void UCombatAbility_Combo::StartNewWaitTaskForInputWindowEnd()
{
	if (WaitForInputWindowEndTask.IsValid())
	{
		WaitForInputWindowStartTask->EndTask();
	}
	WaitForInputWindowEndTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this, FCombatSystem_GameplayTags::Get().AttackInputWindow_End,true);
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
	UEinarGameplayLibrary::RotateToMoveInput(CurrentActorInfo->AvatarActor.Get(),PlayerController->GetMoveInput(), PlayerController->GetControlRotation().Yaw);

}



