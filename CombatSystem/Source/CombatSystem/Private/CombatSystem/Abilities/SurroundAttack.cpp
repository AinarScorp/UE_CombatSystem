// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/SurroundAttack.h"

#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"
#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"
#include "CombatSystem/Tasks/CombatTask_SummonProjectiles.h"
#include "Tags/CombatSystem_GameplayTags.h"

void USurroundAttack::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, TriggerEventData);
	if (AttackMontage)
	{
		UCombatSystem_PlayMontage* MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, "AttackCombo",AttackMontage, 1);
		MontageTask->OnCompleted.AddDynamic(this, &USurroundAttack::InternalEndAbility);
		MontageTask->ReadyForActivation();
	}
	UCombatSystem_WaitGameplayEvent* WaitForInputWindowStartTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(CurrentActorInfo->AvatarActor.Get(),FCombatSystem_GameplayTags::Get().Actions_Attack_Magic,true);
	WaitForInputWindowStartTask->EventReceived.AddDynamic(this, &USurroundAttack::SummonProjectiles);
	WaitForInputWindowStartTask->ReadyForActivation();
}

void USurroundAttack::SummonProjectiles(FCombatEventData Payload)
{
	UCombatTask_SummonProjectiles* SummonProjectilesTask = UCombatTask_SummonProjectiles::SummonProjectiles(CurrentActorInfo->AvatarActor.Get(),ProjectileInfo,NoiseCheckValue,MagicAttackRadius,MagicProjectilesCount);
	SummonProjectilesTask->ReadyForActivation();
}
