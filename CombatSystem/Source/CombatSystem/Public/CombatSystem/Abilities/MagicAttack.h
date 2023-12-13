// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Abilities/CombatAbility.h"
#include "Helpers/HelperStructs.h"
#include "MagicAttack.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UMagicAttack : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Settings")
	float MagicAttackRadius = 100;
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Settings")
	TEnumAsByte<ETraceTypeQuery> EnemyChannel;
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Debug")
	FTraceDebugInfo TraceDebugInfo;
};
