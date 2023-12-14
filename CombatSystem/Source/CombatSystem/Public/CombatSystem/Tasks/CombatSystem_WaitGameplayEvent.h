// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTask.h"
#include "Components/Actor/CombatSystemComponent.h"
#include "CombatSystem_WaitGameplayEvent.generated.h"

class UCombatSystemComponent;
/**
 * 
 */
//TODO: Should I end action if I finished it prematurely, look into Lyra
UCLASS()
class COMBATSYSTEM_API UCombatSystem_WaitGameplayEvent : public UGameplayTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability", meta = (DefaultToSelf = "TargetActor"))
	static UCombatSystem_WaitGameplayEvent* WaitGameplayEvent(UCombatAbility* OwningAbility, FGameplayTag EventTag, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventReceivedDelegate, FCombatEventData, Payload);

	UPROPERTY(BlueprintAssignable)
	FEventReceivedDelegate EventReceived;
protected:
	virtual void Activate() override;
	virtual void EndAction();
	virtual void OnDestroy(bool bInOwnerFinished) override;

	virtual void GameplayEventCallback(const FCombatEventData* Payload);
	virtual void GameplayEventContainerCallback(FGameplayTag MatchingTag, const FCombatEventData* Payload);

	FGameplayTag Tag;
	bool OnlyTriggerOnce = false;
	bool OnlyMatchExact = false;

	FDelegateHandle MyHandle;
private:
	TWeakObjectPtr<UCombatSystemComponent> CombatSystemComponent;

};
