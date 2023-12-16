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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AdditionalInfoTag;

	/** The instigator of the event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<const AActor> Instigator;

	/** The target of the event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<const AActor> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;
};
