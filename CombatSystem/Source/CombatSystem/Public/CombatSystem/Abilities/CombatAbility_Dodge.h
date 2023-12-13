// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "Helpers/GameplayLibraryHelper.h"
#include "Helpers/HelperStructs.h"
#include "CombatAbility_Dodge.generated.h"


USTRUCT(BlueprintType)
struct FDodgeMontage
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FMontageWithSection Montage;
	UPROPERTY(EditDefaultsOnly, meta=(Bitmask, BitmaskEnum = "/Script/CombatSystem.ERelativeContext"))
	int32 Direction;
};
/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatAbility_Dodge : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData) override;
private:
	FMontageWithSection* GetDodgeMontage();
	FMontageWithSection* GetDodgeMontageFromLocking();
	FMontageWithSection* GetDodgeMontage(ERelativeContext RelativeContext);
	bool RotateToMoveInput() const;
private:
	UPROPERTY(EditDefaultsOnly, Category="Dodge|Settings")
	TArray<FDodgeMontage> DodgeMontages;
	UPROPERTY(EditDefaultsOnly, Category="Dodge|Settings")
	float AdditionalMovement = 100;
	
};
