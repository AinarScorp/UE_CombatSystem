// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CombatEventData.generated.h"

USTRUCT(BlueprintType)
struct FCombatEventData
{
	GENERATED_BODY()

	FCombatEventData()
		: Instigator(nullptr)
		, Target(nullptr){}
	
	/** Tag of the event that triggered this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EventTag;

	/** The instigator of the event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const AActor> Instigator;

	/** The target of the event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const AActor> Target;
};
