// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Structs/CombatEventData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatSystem_AbilityLibrary.generated.h"

class UCombatSystemComponent;
struct FGameplayTag;
struct FCombatEventData;
/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_AbilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "CombatSystem|Ability")
    static UCombatSystemComponent* GetCombatSystemComponent(AActor *Actor);

	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability", Meta = (Tooltip = "This function can be used to trigger an ability on the actor in question with useful payload data."))
	static void SendGameplayEventToActor(AActor* Actor, FGameplayTag EventTag, FCombatEventData Payload = FCombatEventData());
	
};
