// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Abilities/CombatAbility.h"
#include "CombatAbility_Hit.generated.h"

//TODO: Combine with Combo animations

USTRUCT()
struct FHitMontageWithSection
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	FName SectionName;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> HitMontage;
};
/**
 * 
 */
UCLASS(Abstract)
class COMBATSYSTEM_API UCombatAbility_Hit : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData) override;

private:
	FHitMontageWithSection GetSectionNameFromContext(const FCombatEventData* TriggerEventData) const;
private:
	UPROPERTY(EditDefaultsOnly)
	FHitMontageWithSection HitFromFrontMontage;
	UPROPERTY(EditDefaultsOnly)
	FHitMontageWithSection HitFromBehindMontage;
	UPROPERTY(EditDefaultsOnly)
	FHitMontageWithSection HitFromRightMontage;
	UPROPERTY(EditDefaultsOnly)
	FHitMontageWithSection HitFromLeftMontage;

};
