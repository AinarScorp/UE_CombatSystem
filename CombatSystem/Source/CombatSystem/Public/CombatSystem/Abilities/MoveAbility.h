// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAbility.h"
#include "CombatSystem/Abilities/CombatAbility.h"
#include "MoveAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class COMBATSYSTEM_API UMoveAbility : public UInputAbility
{
	GENERATED_BODY()
public:
	UMoveAbility();
	virtual void OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo) override;

private:
	UFUNCTION()
	void ExecuteMove(const FInputActionValue& InputActionValue);

};
