// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Structs/CombatAbilitySpec.h"
#include "CombatSystem/Abilities/CombatAbility.h"

bool FCombatAbilitySpec::IsActive() const
{
	return Ability != nullptr && ActiveCount > 0;
}

FCombatAbilitySpec::FCombatAbilitySpec(TSubclassOf<UCombatAbility> InAbilityClass, UObject* InSourceObject)
: Ability(InAbilityClass ? InAbilityClass.GetDefaultObject() : nullptr)
, SourceObject(InSourceObject)
, ActiveCount(0)
, InputPressed(false)
, RemoveAfterActivation(false)
, PendingRemove(false)
, bActivateOnce(false)
{
	Handle.GenerateNewHandle();
}


FCombatAbilitySpec::FCombatAbilitySpec(UCombatAbility* InAbility, UObject* InSourceObject)
: Ability(InAbility)
, SourceObject(InSourceObject)
, ActiveCount(0)
, InputPressed(false)
, RemoveAfterActivation(false)
, PendingRemove(false)
, bActivateOnce(false) 
{
	Handle.GenerateNewHandle();
}


void FCombatAbilitySpecContainer::RegisterWithOwner(UCombatSystem_AbilityComponent* InOwner)
{
	Owner = InOwner;
}
