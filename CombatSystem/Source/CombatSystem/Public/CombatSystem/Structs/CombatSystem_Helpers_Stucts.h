// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CombatSystem_Helpers_Stucts.generated.h"
class UCombatAbility;

USTRUCT(BlueprintType)
struct FAnimMontageInfo
{
	GENERATED_BODY()
	UPROPERTY()
	TObjectPtr<UAnimMontage> AnimMontage;
	UPROPERTY()
	TWeakObjectPtr<UCombatAbility> AnimatingAbility;
};
USTRUCT(BlueprintType)
struct FGameplayTagContainerWithCount
{
public:
	GENERATED_BODY()
	
	void RemoveTags(const FGameplayTagContainer& Container);
	void AddTags(const FGameplayTagContainer& Container);
	
	const FGameplayTagContainer& GetTags() const {return ExplicitTags;}
private:
	
	bool UpdateExplicitTags(const FGameplayTag& Tag, const bool ToIncrease);
private:
	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, int32> ExplicitTagCountMap;

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer ExplicitTags;
};
/**
 * 
 */
