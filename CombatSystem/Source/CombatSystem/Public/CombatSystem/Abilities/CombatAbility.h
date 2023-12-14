// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CombatSystem/Structs/CombatAbilityActorInfo.h"
#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"
#include "Components/Actor/CombatSystem_AbilityComponent.h"
#include "CombatAbility.generated.h"


class UCombatSystem_WaitGameplayEvent;

UENUM(BlueprintType)
enum ECombatAbilityTriggerSource : uint8
{
	// Triggered from a gameplay event, will come with payload
	GameplayEvent,

	// Triggered if the ability's owner gets a tag added, triggered once whenever it's added
	OwnedTagAdded,

	// Triggered if the ability's owner gets tag added, removed when the tag is removed
	OwnedTagPresent,
};

USTRUCT()
struct FCombatAbilityTriggerData
{
	GENERATED_USTRUCT_BODY()

	FCombatAbilityTriggerData()
		: TriggerSource(ECombatAbilityTriggerSource::GameplayEvent)
	{
	}

	/** The tag to respond to */
	UPROPERTY(EditAnywhere, Category=TriggerData, meta=(Categories="TriggerTagCategory"))
	FGameplayTag TriggerTag;

	/** The type of trigger to respond to */
	UPROPERTY(EditAnywhere, Category=TriggerData)
	TEnumAsByte<ECombatAbilityTriggerSource> TriggerSource;
};
DECLARE_MULTICAST_DELEGATE(FOnCombatAbilityCancelled);
/**
 * 
 */
//TODO: Add on remove ability
UCLASS(Abstract, Blueprintable, BlueprintType)
class COMBATSYSTEM_API UCombatAbility : public UObject
{
	GENERATED_BODY()
	friend class UCombatSystem_AbilityComponent;
public:
	UCombatAbility(const FObjectInitializer& ObjectInitializer);
public:
	/** Returns true if this ability can be activated right now. Has no side effects */
	virtual bool CanActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const;
	/** Returns true if none of the ability's tags are blocked and if it doesn't have a "Blocking" tag and has all "Required" tags. */
	virtual bool DoesAbilitySatisfyTagRequirements(const UCombatSystem_AbilityComponent& CombatAbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const;

	/** Checks cooldown. returns true if we can be used again. False if not */
	virtual bool CheckCooldown(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	/** Checks cost. returns true if we can pay for the ability. False if not */
	virtual bool CheckCost(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData = nullptr);
	virtual void EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled);
	virtual void CancelAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);

	virtual void OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);
	/** Input binding stub. */
	virtual void InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);
	/** Input binding stub. */
	virtual void InputReleased(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "ActivateAbility")
	void BP_ActivateAbility();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "CanActivateAbility")
	bool BP_CanActivateAbility(FCombatAbilityActorInfo ActorInfo) const;
	bool bHasBlueprintCanUse;
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnEndAbility")
	void BP_OnEndAbility(bool bWasCancelled);
	UFUNCTION(BlueprintCallable, DisplayName = "EndAbility")
	void BP_EndAbility();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInputPressed")
	void BP_OnInputPressed();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInputReleased")
	void BP_OnInputReleased();

	UFUNCTION(BlueprintCallable, Category = Animation)
	UAnimMontage* GetCurrentMontage() const;
	virtual void SetCurrentMontage(class UAnimMontage* InCurrentMontage);

	/** Returns the actor info associated with this ability, has cached pointers to useful objects */
	UFUNCTION(BlueprintCallable, Category="CombatSystem|Ability")
	FCombatAbilityActorInfo GetActorInfo() const { return *CurrentActorInfo; };
	UFUNCTION(BlueprintCallable, Category="CombatSystem|Ability")
	FCombatEventData GetCombatEventData() const { return CombatEventData; };
	const FCombatAbilityActorInfo* GetCurrentActorInfo() const { return CurrentActorInfo; };
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability")
	FGameplayTagContainer GetAbilityTags() const { return AbilityTags; };
protected:
	UFUNCTION()
	virtual void InternalEndAbility();
	UFUNCTION()
	virtual void InternalCancelAbility();
	// template<class UserClass,class FuncName>
	// UCombatSystem_PlayMontage* StartMontageTask(UserClass* Object, UAnimMontage* MontageToPlay, FName StartSection, FuncName OnCompletedFunction = nullptr,FuncName OnInterruptedFunc = nullptr);

public:
	FOnCombatAbilityCancelled OnCombatAbilityCancelled;

	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer AbilityTags;
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta=(Categories="AbilityTagCategory"))
	FGameplayTagContainer BlockAbilitiesWithTag;
	/** Abilities with these tags are cancelled when this ability is executed */
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta=(Categories="AbilityTagCategory"))
	FGameplayTagContainer CancelAbilitiesWithTag;
	/** This ability is blocked if the source actor/component has any of these tags */
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta=(Categories="SourceTagsCategory"))
	FGameplayTagContainer SourceBlockedTags;
	/** This ability is blocked if the target actor/component has any of these tags */
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta=(Categories="TargetTagsCategory"))
	FGameplayTagContainer TargetBlockedTags;
	
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer OnGiveAbilityGrandTags;

	

protected:
	mutable const FCombatAbilityActorInfo* CurrentActorInfo;
	mutable FCombatAbilitySpecHandle CurrentSpecHandle;
	FCombatEventData CombatEventData;

	UPROPERTY(EditDefaultsOnly, Category = Triggers)
	TArray<FCombatAbilityTriggerData> AbilityTriggers;
	UPROPERTY(EditDefaultsOnly, Category = Advanced)
	bool bRetriggerInstancedAbility;
	UPROPERTY(EditDefaultsOnly)
	bool bIsCancelable = true;
	UPROPERTY()
	bool bIsActive;
	UPROPERTY()
	TObjectPtr<class UAnimMontage> CurrentMontage;
};
// template <class UserClass, typename FuncName>
// UCombatSystem_PlayMontage* UCombatAbility::StartMontageTask(UserClass* Object,UAnimMontage* MontageToPlay, FName StartSection, FuncName OnCompletedFunction, FuncName OnInterruptedFunc)
// {
// 	UCombatSystem_PlayMontage* MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, GetFName(), MontageToPlay, 1, StartSection);
// 	if (OnCompletedFunction)
// 	{
// 		MontageTask->OnCompleted.AddDynamic(Object, OnCompletedFunction);
// 	}
// 	if (OnInterruptedFunc)
// 	{
// 		MontageTask->OnInterrupted.AddDynamic(Object,OnInterruptedFunc);
// 	}
// 	MontageTask->ReadyForActivation();
// 	return MontageTask;
// }
