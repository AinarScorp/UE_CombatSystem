// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/InputAbility.h"
#include "Data/CombatSystem_CharacterStartupInfo.h"
#include "InputAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UInputAttackAbility : public UInputAbility
{
	GENERATED_BODY()
public:
	virtual void OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo) override;
	virtual void InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo) override;
	UPROPERTY(EditDefaultsOnly)
	TArray<FTagAbility> AbilitiesToGive;
	UPROPERTY(VisibleAnywhere)
	FTagAbility ActiveAbility;
	
};
