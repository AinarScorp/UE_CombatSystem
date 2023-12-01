// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/InputAttackAbility.h"

void UInputAttackAbility::OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::OnGiveAbility(Handle, ActorInfo);
	for (const auto Ability : AbilitiesToGive)
	{
		FCombatAbilitySpec AbilitySpec = FCombatAbilitySpec(Ability.Ability);
		AbilitySpec.DynamicAbilityTags.AddTag(Ability.DynamicTag);
		ActorInfo->CombatAbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

void UInputAttackAbility::InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::InputPressed(Handle, ActorInfo);


	FCombatAbilitySpec* Spec = ActorInfo->CombatAbilitySystemComponent->FindAbilitySpecFromClass(ActiveAbility.Ability);
	//Spec->InstancedAbility->InputPressed();
}
