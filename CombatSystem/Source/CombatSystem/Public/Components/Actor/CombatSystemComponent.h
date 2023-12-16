// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTasksComponent.h"
#include "GameplayTagContainer.h"
#include "CombatSystem/Structs/CombatAbilitySpec.h"
#include "CombatSystem/Structs/CombatAbilitySpecHandle.h"
#include "CombatSystem/Structs/CombatEventData.h"
#include "CombatSystem/Structs/CombatSystem_Helpers_Stucts.h"
#include "CombatSystemComponent.generated.h"


class UInputAbility;
struct FCombatAbilityActorInfo;
class UCombatAbility;
class UGameplayAbility;
class UCombatSystemComponent;
struct FGameplayTag;




/** Delegate for handling gameplay event data */

DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayEventMulticastDelegate, const FCombatEventData*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded, UCombatAbility*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded_BP, UCombatAbility*, CombatAbility);

UCLASS()
class COMBATSYSTEM_API UCombatSystemComponent : public UGameplayTasksComponent
{
	GENERATED_BODY()
public:
	
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);
	virtual void OnRegister() override;
	FCombatAbilitySpecHandle GiveAbility(const FCombatAbilitySpec& AbilitySpec);
	UFUNCTION(BlueprintCallable, Category = "Abilities", meta = (DisplayName = "Give Ability"))
	FCombatAbilitySpecHandle GiveAbility_BP(TSubclassOf<UCombatAbility> AbilityClass);
	
	// Handle Abilities with Input
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagPressedWithValue(const FInputActionValue& InputActionValue,const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagReleasedWithValue(const FInputActionValue& InputActionValue,const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void AbilitySpecInputPressed(FCombatAbilitySpec& Spec) const;
	void AbilitySpecInputReleased(FCombatAbilitySpec& Spec) const;
	
	//ActivateAbility
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbility(FCombatAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation = true);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation = true);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilitiesByClass(TSubclassOf<UCombatAbility> InAbilityToActivate, bool bAllowRemoteActivation = true);
	bool InternalTryActivateAbility(FCombatAbilitySpecHandle AbilityToActivate, const FCombatEventData* TriggerEventData = nullptr);
	
	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FCombatEventData* Payload);
	bool TriggerAbilityFromGameplayEvent(FCombatAbilitySpecHandle AbilityToTrigger, FCombatAbilityActorInfo* ActorInfo, FGameplayTag Tag, const FCombatEventData* Payload, UCombatSystemComponent& Component);

	//When Ability is Activated they should call these functions
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UCombatAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags);
	void ApplyAbilityContainedTags(const FGameplayTagContainer& AbilityTags, bool bRemove);

	//Cancel  & End Abilities Functions
	void CancelAbilities(const FGameplayTagContainer* WithTags=nullptr, const FGameplayTagContainer* WithoutTags=nullptr, UCombatAbility* Ignore=nullptr);
	virtual void CancelAbilitySpec(FCombatAbilitySpec& Spec, UCombatAbility* Ignore);
	virtual void NotifyAbilityEnded(FCombatAbilitySpecHandle Handle, UCombatAbility* Ability, bool bWasCancelled);

	//Helper functions
	FCombatAbilitySpec* FindAbilitySpecFromHandle(FCombatAbilitySpecHandle Handle);
	FCombatAbilitySpec* FindAbilitySpecFromClass(TSubclassOf<UCombatAbility> InAbilityClass);
	void GetActivatableAbilitySpecsByAllMatchingTags(const FGameplayTagContainer& GameplayTagContainer, TArray<FCombatAbilitySpec*>& MatchingGameplayAbilities, bool bOnlyAbilitiesThatSatisfyTagRequirements = true) const;
	virtual bool AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const;
	virtual bool ContainsAbilityTags(const FGameplayTagContainer& Tags) const;


	//Animation Related
	virtual float PlayMontage(UCombatAbility* AnimatingAbility, UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName = NAME_None, float StartTimeSeconds = 0.0f);
	virtual void CurrentMontageStop(float OverrideBlendOutTime = -1.0f);
	virtual void ClearAnimatingAbility(UCombatAbility* Ability);
	UCombatAbility* GetAnimatingAbility() const;
	UAnimMontage* GetCurrentMontage() const;
	//Getters & Setters
	FORCEINLINE const FGameplayTagContainer& GetContainedAbilityTags() const {return ContainedTags.GetTags();};

private:
	void RegisterTriggerableAbilities(const FCombatAbilitySpec& AbilitySpec);
public:
	TSharedPtr<FCombatAbilityActorInfo>	CombatAbilityActorInfo;
	
	//Delegates
	TMap<FGameplayTag, FGameplayEventMulticastDelegate> GenericGameplayEventCallbacks;
	FOnAbilityEnded AbilityEndedCallbacks;
	UPROPERTY(BlueprintAssignable, DisplayName = "AbilityEndedCallbacks")
	FOnAbilityEnded_BP AbilityEndedCallbacks_BP;
private:
	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainerWithCount BlockedTags;
	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainerWithCount ContainedTags;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "CombatSystem|Abilities", meta = (AllowPrivateAccess = "true"))
	FCombatAbilitySpecContainer ActivatableAbilities;
	UPROPERTY()
	FAnimMontageInfo AnimMontageInfo {};
	
	TMap<FGameplayTag, TArray<FCombatAbilitySpecHandle>> GameplayEventTriggeredAbilities;
	
	//Input spec handles are used to trigger abilities from Inputs
	TArray<FCombatAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FCombatAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FCombatAbilitySpecHandle> InputHeldSpecHandles;
	


};
