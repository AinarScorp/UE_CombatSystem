// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTask.h"
#include "Components/Actor/CombatSystem_AbilityComponent.h"
#include "CombatSystem_WaitGameplayEvent.generated.h"

class UCombatSystem_AbilityComponent;
/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_WaitGameplayEvent : public UGameplayTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability", meta = (DefaultToSelf = "TargetActor"))
	static UCombatSystem_WaitGameplayEvent* WaitGameplayEvent(AActor* TargetActor, FGameplayTag EventTag, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventReceivedDelegate, FCombatEventData, Payload);

	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate EventReceived;
protected:
	virtual void Activate() override;
	virtual void EndAction();

	virtual void GameplayEventCallback(const FCombatEventData* Payload);
	virtual void GameplayEventContainerCallback(FGameplayTag MatchingTag, const FCombatEventData* Payload);

	FGameplayTag Tag;
	bool OnlyTriggerOnce = false;
	bool OnlyMatchExact = false;

	FDelegateHandle MyHandle;
private:
	TWeakObjectPtr<UCombatSystem_AbilityComponent> CombatSystemComponent;

};
