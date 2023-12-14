// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/InputAttackAbility.h"

void UInputAttackAbility::OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::OnGiveAbility(Handle, ActorInfo);
	for (const auto Ability : AbilitiesToGive)
	{
		FCombatAbilitySpec AbilitySpec = FCombatAbilitySpec(Ability.Ability);
		AbilitySpec.DynamicAbilityTags.AddTag(Ability.DynamicTag);
		ActorInfo->CombatSystemComponent->GiveAbility(AbilitySpec);
	}
}

void UInputAttackAbility::InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::InputPressed(Handle, ActorInfo);


	FCombatAbilitySpec* Spec = ActorInfo->CombatSystemComponent->FindAbilitySpecFromClass(ActiveAbility.Ability);
	//Spec->InstancedAbility->InputPressed();
}
