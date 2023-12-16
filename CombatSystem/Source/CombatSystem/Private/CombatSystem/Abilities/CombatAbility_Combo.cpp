// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/CombatAbility_Combo.h"

#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"
#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"
#include "Library/EinarGameplayLibrary.h"
#include "Player/Controller/CombatSystem_PlayerController.h"
#include "Tags/CombatSystem_GameplayTags.h"



void UCombatAbility_Combo::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	 bool ShouldPerfromAttackImmidiately = true;
	 const FGameplayTagContainer ComboTagContainer{FCombatSystem_GameplayTags::Get().Actions_Attack_Combo};
	 if (ActorInfo->CombatSystemComponent->ContainsAbilityTags(ComboTagContainer))
	 {
	 	ShouldPerfromAttackImmidiately = false;
	 	bReceivedInput = true;
	 }
	 
	Super::ActivateAbility(Handle, ActorInfo,TriggerEventData);

	ActivateWaitTasks(ShouldPerfromAttackImmidiately);
	
	if (!ShouldPerfromAttackImmidiately) return;
	PerformAttack(FirstAttack);
	
}

void UCombatAbility_Combo::EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, bWasCancelled);
	bReceivedInput = false;
	NextAttackAnimation = nullptr;
}


void UCombatAbility_Combo::TryPerformAttack()
{
	if (!bReceivedInput ||!NextAttackAnimation)
	{
		return;
	}
	PerformAttack(*NextAttackAnimation);
}

void UCombatAbility_Combo::PerformAttack(const FMontageWithSection& AttackMontage)
{
	PlayAttackAnimation(AttackMontage);
	RotateToMoveInput();
	//Reset values
	bReceivedInput = false;
	NextAttackAnimation = nullptr;
}

void UCombatAbility_Combo::ActivateWaitTasks(const bool WaitToStartListeningForInput)
{
	if (WaitToStartListeningForInput)
	{
		UCombatSystem_WaitGameplayEvent* WaitForInputWindowStartTask =UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this,FCombatSystem_GameplayTags::Get().AttackInputWindow_Start,true);
		WaitForInputWindowStartTask->EventReceived.AddDynamic(this,&ThisClass::StartListeningToInput);
		WaitForInputWindowStartTask->ReadyForActivation();
	}
	
	UCombatSystem_WaitGameplayEvent* WaitForInputWindowEndTask =UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this,FCombatSystem_GameplayTags::Get().AttackInputWindow_End,true);
	WaitForInputWindowEndTask->EventReceived.AddDynamic(this,&ThisClass::StopListeningToInput);
	WaitForInputWindowEndTask->ReadyForActivation();
	
	WaitForInputTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this,ActivatedByInputTag,false);
	WaitForInputTask->EventReceived.AddDynamic(this,&ThisClass::ReceivedInput);
	WaitForInputTask->ReadyForActivation();
	
	WaitForAnimationReadyTask =UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(this,FCombatSystem_GameplayTags::Get().Animation_NextAttackReady,false);
	WaitForAnimationReadyTask->EventReceived.AddDynamic(this,&ThisClass::ReceivedAnimation);
	WaitForAnimationReadyTask->ReadyForActivation();
}

void UCombatAbility_Combo::ReceivedInput(FCombatEventData Payload)
{
	bReceivedInput = true;
	TryPerformAttack();
}

void UCombatAbility_Combo::ReceivedAnimation(FCombatEventData Payload)
{
	for (auto& Combo : Combos)
	{
		if (Combo.TriggerTag != Payload.AdditionalInfoTag) continue;
		NextAttackAnimation = &Combo;
	}
	if (!NextAttackAnimation)
	{
		InternalEndAbility();
		return;
	}
	TryPerformAttack();
}

void UCombatAbility_Combo::StartListeningToInput(FCombatEventData Payload)
{
	bReceivedInput = false;
}

void UCombatAbility_Combo::StopListeningToInput(FCombatEventData Payload)
{
	//Stopped here
	// if (!MontageTask.IsValid())
	// {
	// 	TryPerformAttack();
	// 	if (MontageTask.IsValid())
	// 	{
	// 		return;
	// 	}
	// }
	if (WaitForAnimationReadyTask.IsValid())
	{
		WaitForAnimationReadyTask->ExternalCancel();
	}
	if (WaitForInputTask.IsValid())
	{
		WaitForInputTask->ExternalCancel();
	}
}


void UCombatAbility_Combo::AttackInputWindowStarted(FCombatEventData Payload)
{
	ListeningToInput = true;
}

void UCombatAbility_Combo::AttackInputWindowEnded(FCombatEventData Payload)
{
	//StartNextAttack();
}

void UCombatAbility_Combo::RotateToMoveInput() const
{
	if (!bShouldRotateToInput || !CurrentActorInfo->Controller.Get()) return;
	const ACombatSystem_PlayerController* PlayerController = Cast<ACombatSystem_PlayerController>(CurrentActorInfo->Controller);
	if (!PlayerController) return;
	UEinarGameplayLibrary::RotateToMoveInput(CurrentActorInfo->AvatarActor.Get(),PlayerController->GetMoveInput(), PlayerController->GetControlRotation().Yaw);

}

void UCombatAbility_Combo::PlayAttackAnimation(const FMontageWithSection& AttackMontage)
{
	if (MontageTask.Get())
	{
		MontageTask->OnInterrupted.RemoveDynamic(this, &UCombatAbility_Combo::InternalCancelAbility);
	}
	MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, GetFName(), AttackMontage.AnimMontage, 1, AttackMontage.AnimSection,false);
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbility_Combo::InternalEndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UCombatAbility_Combo::InternalCancelAbility);
	MontageTask->ReadyForActivation();
	


}



