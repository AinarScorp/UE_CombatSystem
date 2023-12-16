// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/CombatAbility_ComboInput.h"

#include "CombatSystem/Library/CombatSystem_AbilityLibrary.h"
#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"
#include "Tags/CombatSystem_GameplayTags.h"



void UCombatAbility_ComboInput::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, TriggerEventData);
	UCombatSystem_AbilityLibrary::SendGameplayEventToActor(ActorInfo->AvatarActor.Get(),TriggerAbilitiesWithTag);
	WaitForInputWindowStartTask =UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this,FCombatSystem_GameplayTags::Get().AttackInputWindow_Start,true, true);
	WaitForInputWindowStartTask->EventReceived.AddDynamic(this, &ThisClass::OnAnimationSendInput);
	WaitForInputWindowStartTask->ReadyForActivation();
	WaitForInputWindowEndTask =UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this,FCombatSystem_GameplayTags::Get().AttackInputWindow_End,true, true);
	WaitForInputWindowEndTask->EventReceived.AddDynamic(this, &ThisClass::OnAnimationSendInput);
	WaitForInputWindowEndTask->ReadyForActivation();
}

void UCombatAbility_ComboInput::OnAnimationSendInput(FCombatEventData Payload)
{
	// if (Payload.EventTag == FCombatSystem_GameplayTags::Get().AttackInputWindow_Start)
	// {
	// 	WaitForInputWindowEndTask->ExternalCancel();
	// }
	// else if (Payload.EventTag == FCombatSystem_GameplayTags::Get().AttackInputWindow_End)
	// {
	// 	WaitForInputWindowStartTask->ExternalCancel();
	// }
	InternalEndAbility();
}
