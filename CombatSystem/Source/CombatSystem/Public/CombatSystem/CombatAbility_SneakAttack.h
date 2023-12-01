// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/CombatAbility.h"
#include "Helpers/HelperStructs.h"
#include "CombatAbility_SneakAttack.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBATSYSTEM_API UCombatAbility_SneakAttack : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual bool CanActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const override;
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData) override;
	virtual void EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "SneakAttack|Settings")
	UAnimMontage* StealthKillMontage;
	UPROPERTY(EditDefaultsOnly, Category = "SneakAttack|Settings")
	float SneakAttackRadius = 100;
	UPROPERTY(EditDefaultsOnly, Category = "SneakAttack|Settings")
	float SneakAttackVisionAngle = 100;
	UPROPERTY(EditDefaultsOnly, Category = "SneakAttack|Settings")
	TEnumAsByte<ETraceTypeQuery> EnemyChannel;
	UPROPERTY(EditDefaultsOnly, Category="SneakAttack|Settings", meta=(Bitmask, BitmaskEnum = "/Script/CombatSystem.ERelativeContext"))
	int32 Context;
	UPROPERTY(EditDefaultsOnly, Category = "SneakAttack|Debug")
	FTraceDebugInfo TraceDebugInfo;

	UPROPERTY(EditDefaultsOnly,  Category = "SneakAttack|Optimization")
	float ConeCastRayCount = 10;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	mutable TObjectPtr<AActor> Target;
};
