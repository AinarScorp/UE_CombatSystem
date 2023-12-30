// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CombatSystem/Structs/CombatAbilityActorInfo.h"
#include "Components/Actor/CombatSystemComponent.h"
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
class COMBATSYSTEM_API UCombatAbility : public UObject, public IGameplayTaskOwnerInterface
{
	GENERATED_BODY()
	friend class UCombatSystemComponent;
public:
	UCombatAbility(const FObjectInitializer& ObjectInitializer);
public:

	// **IGameplayTaskOwnerInterface
	virtual UGameplayTasksComponent* GetGameplayTasksComponent(const UGameplayTask& Task) const override;
	virtual AActor* GetGameplayTaskAvatar(const UGameplayTask* Task) const override;
	virtual AActor* GetGameplayTaskOwner(const UGameplayTask* Task) const override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;
	
	virtual void OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData = nullptr);
	virtual void EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled);
	virtual void CancelAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);
	
	virtual bool CanActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const;
	virtual bool DoesAbilitySatisfyTagRequirements(const UCombatSystemComponent& CombatAbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const;
	
	virtual void InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);
	virtual void InputReleased(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo);
	
	const FCombatAbilityActorInfo* GetCurrentActorInfo() const { return CurrentActorInfo; };

	//**Blueprint Implementable Functions
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "ActivateAbility")
	void BP_ActivateAbility();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "CanActivateAbility")
	bool BP_CanActivateAbility(FCombatAbilityActorInfo ActorInfo) const;
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnEndAbility")
	void BP_OnEndAbility(bool bWasCancelled);
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInputPressed")
	void BP_OnInputPressed();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInputReleased")
	void BP_OnInputReleased();
	
	//**Blueprint Callable Functions
	UFUNCTION(BlueprintCallable, DisplayName = "EndAbility")
	void BP_EndAbility();
	UFUNCTION(BlueprintCallable, Category="CombatSystem|Ability")
	FCombatAbilityActorInfo GetActorInfo() const { return *CurrentActorInfo; }
	UFUNCTION(BlueprintCallable, Category="CombatSystem|Ability")
	FCombatEventData GetCombatEventData() const { return CombatEventData; }
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability")
	FGameplayTagContainer GetAbilityTags() const { return AbilityTags; };
	
	//Animation Related Functions
	UFUNCTION(BlueprintCallable, Category = Animation)
	UAnimMontage* GetCurrentMontage() const;
	virtual void SetCurrentMontage(class UAnimMontage* InCurrentMontage);

protected:
	UFUNCTION()
	virtual void InternalEndAbility();
	UFUNCTION()
	virtual void InternalCancelAbility();

public:
	//**Delegates
	FOnCombatAbilityCancelled OnCombatAbilityCancelled;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer AbilityTags;
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta=(Categories="AbilityTagCategory"))
	FGameplayTagContainer BlockAbilitiesWithTag;
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
	UPROPERTY(EditDefaultsOnly, Category = "CombatSystem|Ability|Triggers")
	TArray<FCombatAbilityTriggerData> AbilityTriggers;
	UPROPERTY(EditDefaultsOnly, Category = "CombatSystem|Ability|Advanced", meta = (cate))
	bool bRetriggerInstancedAbility;
	UPROPERTY(EditDefaultsOnly, Category="CombatSystem|Ability")
	bool bIsCancelable = true;
	UPROPERTY()
	bool bIsActive;
	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentMontage;
	UPROPERTY()
	TArray<TObjectPtr<UGameplayTask>> ActiveTasks;
	
	mutable const FCombatAbilityActorInfo* CurrentActorInfo;
	mutable FCombatAbilitySpecHandle CurrentSpecHandle;
	FCombatEventData CombatEventData;
private:
	bool bCanActivateAbilityImplementedInBP;
protected:
	UPROPERTY(EditDefaultsOnly)
	FColor DebugColor = FColor::Yellow;
	UPROPERTY(EditDefaultsOnly)
	FString DebugName = "Combo #";
};


// template<class UserClass,class FuncName>
// UCombatSystem_PlayMontage* StartMontageTask(UserClass* Object, UAnimMontage* MontageToPlay, FName StartSection, FuncName OnCompletedFunction = nullptr,FuncName OnInterruptedFunc = nullptr);

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
