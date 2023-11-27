// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CombatSystem_CharacterStartupInfo.generated.h"


class UCombatAbility;

USTRUCT()
struct FTagAbility
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCombatAbility> Ability;
	UPROPERTY(EditAnywhere)
	FGameplayTag DynamicTag;
};

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_CharacterStartupInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	TArray<FTagAbility> GetAbilityList(){return Abilities;};
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> GetAnimMontages() {return AnimMontagesByTag;};

private:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FTagAbility> Abilities;
	UPROPERTY(EditDefaultsOnly, Category = "Animations With Tag")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> AnimMontagesByTag;

};
