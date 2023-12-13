// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Abilities/CombatAbility.h"
#include "CombatSystem/Tasks/CombatTask_SummonProjectiles.h"
#include "SurroundAttack.generated.h"

/**
 * 
 */

UCLASS()
class COMBATSYSTEM_API USurroundAttack : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData) override;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteProjectileExplosion(FVector Location);
private:
	UFUNCTION()
	void SummonProjectiles(FCombatEventData Payload);
private:
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Settings")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Settings")
	FProjectileInfo ProjectileInfo;
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Settings")
	float NoiseCheckValue = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Settings")
	float MagicAttackRadius = 100;
	UPROPERTY(EditDefaultsOnly, Category = "MagicAttack|Settings")
	int MagicProjectilesCount = 50;

};
