// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "CombatAbility_ComboInput.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBATSYSTEM_API UCombatAbility_ComboInput : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData) override;
private:
	UFUNCTION()
	void OnAnimationSendInput(FCombatEventData Payload);
private:
	TWeakObjectPtr<UCombatSystem_WaitGameplayEvent> WaitForInputWindowStartTask;
	TWeakObjectPtr<UCombatSystem_WaitGameplayEvent> WaitForInputWindowEndTask;
	UPROPERTY(EditDefaultsOnly, Category = "ComboInput|Tags")
	FGameplayTag TriggerAbilitiesWithTag;
};
