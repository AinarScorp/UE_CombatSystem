// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Library/CombatSystem_AbilityLibrary.h"

#include "Components/Actor/CombatSystemComponent.h"
#include "Interfaces/CombatSystem_AbilityInterface.h"

UCombatSystemComponent* UCombatSystem_AbilityLibrary::GetCombatSystemComponent(AActor* Actor)
{
	const ICombatSystem_AbilityInterface* CombatAbilityInterface = Cast<ICombatSystem_AbilityInterface>(Actor);
	return CombatAbilityInterface? CombatAbilityInterface->GetCombatSystemComponent() : nullptr;
}

void UCombatSystem_AbilityLibrary::SendGameplayEventToActor(AActor* Actor, FGameplayTag EventTag, FCombatEventData Payload)
{
	if (IsValid(Actor))
	{
		UCombatSystemComponent* CombatSystemComponent = GetCombatSystemComponent(Actor);
		if (CombatSystemComponent)
		{
			CombatSystemComponent->HandleGameplayEvent(EventTag, &Payload);
		}
		else
		{
			UE_LOG(LogTemp,Error, TEXT("UCombatSystem_AbilityLibrary::SendGameplayEventToActor: Invalid ability system component retrieved from Actor %s. EventTag was %s"), *Actor->GetName(), *EventTag.ToString())
		}
	}
}
