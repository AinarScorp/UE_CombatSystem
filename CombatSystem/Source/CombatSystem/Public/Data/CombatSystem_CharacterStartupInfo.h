// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CombatSystem_CharacterStartupInfo.generated.h"


class UCombatAbility;

USTRUCT(BlueprintType)
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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FTagAbility> Abilities;

};
