// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CombatSystem_CharacterBase.h"

#include "Components/Actor/CombatSystemComponent.h"
#include "Data/CombatSystem_CharacterStartupInfo.h"

// Sets default values
ACombatSystem_CharacterBase::ACombatSystem_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	CombatSystemComponent = CreateDefaultSubobject<UCombatSystemComponent>("CombatSystemComponent");
}


UCombatSystemComponent* ACombatSystem_CharacterBase::GetCombatSystemComponent() const
{
	return CombatSystemComponent;

}

void ACombatSystem_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	CombatSystemComponent->InitAbilityActorInfo(this,this);
	
	for (auto StartupAbility : StartupInfo.Get()->GetAbilityList())
	{
		FCombatAbilitySpec AbilitySpec = FCombatAbilitySpec(StartupAbility.Ability);
		AbilitySpec.DynamicAbilityTags.AddTag(StartupAbility.DynamicTag);
		CombatSystemComponent->GiveAbility(AbilitySpec);
	}
}
