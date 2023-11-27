// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Structs/CombatAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "CombatAbilitySpec.generated.h"

struct FInputActionValue;
class UCombatAbility;
class UCombatSystem_AbilityComponent;

USTRUCT(BlueprintType)
struct FCombatAbilitySpec
{
	GENERATED_BODY()
	FCombatAbilitySpec()
	: Ability(nullptr), SourceObject(nullptr), ActiveCount(0), InputPressed(false), RemoveAfterActivation(false), PendingRemove(false), bActivateOnce(false)
	{ }
	
	/** Version that takes an ability class */
	FCombatAbilitySpec(TSubclassOf<UCombatAbility> InAbilityClass, UObject* InSourceObject = nullptr);
	
	/** Version that takes an ability CDO, this exists for backward compatibility */
	FCombatAbilitySpec(UCombatAbility* InAbility, UObject* InSourceObject = nullptr);
	

	/** Handle for outside sources to refer to this spec by */
	UPROPERTY(VisibleAnywhere)
	FCombatAbilitySpecHandle Handle;
	
	/** Ability of the spec (Always the CDO. This should be const but too many things modify it currently) */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatAbility> Ability;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatAbility> InstancedAbility;


	/** Object this ability was created from, can be an actor or static object. Useful to bind an ability to a gameplay object */
	UPROPERTY()
	TWeakObjectPtr<UObject> SourceObject;

	/** A count of the number of times this ability has been activated minus the number of times it has been ended. For instanced abilities this will be the number of currently active instances. Can't replicate until prediction accurately handles this.*/
	UPROPERTY()
	uint8 ActiveCount;

	/** Is input currently pressed. Set to false when input is released */
	UPROPERTY()
	uint8 InputPressed:1;

	/** If true, this ability should be removed as soon as it finishes executing */
	UPROPERTY()
	uint8 RemoveAfterActivation:1;

	/** Pending removal due to scope lock */
	UPROPERTY()
	uint8 PendingRemove:1;

	/** This ability should be activated once when it is granted. */
	UPROPERTY()
	uint8 bActivateOnce : 1;
	
	/** Optional ability tags that are replicated.  These tags are also captured as source tags by applied gameplay effects. */
	UPROPERTY()
	FGameplayTagContainer DynamicAbilityTags;
	
	/** Returns true if this ability is active in any way */
	bool IsActive() const;


};
USTRUCT(BlueprintType)
struct FCombatAbilitySpecContainer
{
	GENERATED_BODY()

	FCombatAbilitySpecContainer(): Owner(nullptr)
	{
	}

	/** List of activatable abilities */
	UPROPERTY(VisibleAnywhere)
	TArray<FCombatAbilitySpec> Items;

	/** Component that owns this list */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCombatSystem_AbilityComponent> Owner;

	void RegisterWithOwner(UCombatSystem_AbilityComponent* InOwner);
};
