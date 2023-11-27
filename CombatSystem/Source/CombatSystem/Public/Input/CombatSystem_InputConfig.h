// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CombatSystem_InputConfig.generated.h"
USTRUCT(BlueprintType)
struct FCombatInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};
/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_InputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Pawn|Input")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Pawn|Input")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FCombatInputAction> NativeInputActions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCombatInputAction> AbilityInputActions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCombatInputAction> AbilityInputActionsWithValues;
};
