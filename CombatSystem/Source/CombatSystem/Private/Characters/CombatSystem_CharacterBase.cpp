// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CombatSystem_CharacterBase.h"

#include "Components/Actor/CombatSystem_AbilityComponent.h"
#include "Data/CombatSystem_CharacterStartupInfo.h"

// Sets default values
ACombatSystem_CharacterBase::ACombatSystem_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	CombatSystemComponent = CreateDefaultSubobject<UCombatSystem_AbilityComponent>("CombatSystemComponent");
}

UCombatSystem_AbilityComponent* ACombatSystem_CharacterBase::GetCombatAbilitySystemComponent() const
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

	if (!StartupInfo->GetAnimMontages().IsEmpty())
	{
		CombatSystemComponent->AddAnimMontages(StartupInfo->GetAnimMontages());
	}
}
