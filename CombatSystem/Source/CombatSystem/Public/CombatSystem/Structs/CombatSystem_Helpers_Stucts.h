// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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


/**
 * 
 */
