// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTasksComponent.h"
#include "GameplayTagContainer.h"
#include "CombatSystem/Structs/CombatAbilitySpec.h"
#include "CombatSystem/Structs/CombatAbilitySpecHandle.h"
#include "CombatSystem/Structs/CombatEventData.h"
#include "CombatSystem_AbilityComponent.generated.h"


class UInputAbility;
struct FCombatAbilityActorInfo;
class UCombatAbility;
class UGameplayAbility;
class UCombatSystem_AbilityComponent;
struct FGameplayTag;




/** Delegate for handling gameplay event data */

DECLARE_MULTICAST_DELEGATE_OneParam(FGameplayEventMulticastDelegate, const FCombatEventData*);

//TODO: Rename to CombatAbilitySystemCoponent
UCLASS()
class COMBATSYSTEM_API UCombatSystem_AbilityComponent : public UGameplayTasksComponent
{
	GENERATED_BODY()
public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);
	virtual void OnRegister() override;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagPressedWithValue(const FInputActionValue& InputActionValue,const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagReleasedWithValue(const FInputActionValue& InputActionValue,const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void AbilitySpecInputPressed(FCombatAbilitySpec& Spec) const;
	void AbilitySpecInputReleased(FCombatAbilitySpec& Spec) const;
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbility(FCombatAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation = true);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation = true);
	bool TriggerAbilityFromGameplayEvent(FCombatAbilitySpecHandle AbilityToTrigger, FCombatAbilityActorInfo* ActorInfo, FGameplayTag Tag, const FCombatEventData* Payload, UCombatSystem_AbilityComponent& Component);

	bool InternalTryActivateAbility(FCombatAbilitySpecHandle AbilityToActivate, UCombatAbility ** OutInstancedAbility = nullptr, const FCombatEventData* TriggerEventData = nullptr);
	
	FCombatAbilitySpecHandle GiveAbility(const FCombatAbilitySpec& AbilitySpec);
	void CancelAbilities(const FGameplayTagContainer* WithTags=nullptr, const FGameplayTagContainer* WithoutTags=nullptr, UCombatAbility* Ignore=nullptr);
	virtual void CancelAbilitySpec(FCombatAbilitySpec& Spec, UCombatAbility* Ignore);
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UCombatAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags);

	virtual int32 HandleGameplayEvent(FGameplayTag EventTag, const FCombatEventData* Payload);

	FCombatAbilitySpec* FindAbilitySpecFromHandle(FCombatAbilitySpecHandle Handle);
	void GetActivatableAbilitySpecsByAllMatchingTags(const FGameplayTagContainer& GameplayTagContainer, TArray<FCombatAbilitySpec*>& MatchingGameplayAbilities, bool bOnlyAbilitiesThatSatisfyTagRequirements = true) const;
	virtual bool AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const;

	void AddAnimMontages(const TMap<FGameplayTag, TObjectPtr<UAnimMontage>>& NewMontages);
	virtual float PlayMontage(UCombatAbility* AnimatingAbility, UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName = NAME_None, float StartTimeSeconds = 0.0f);
	UAnimMontage* GetAnimMontageByTag(FGameplayTag ByTag);
private:
	void RegisterTriggerableAbilities(const FCombatAbilitySpec& AbilitySpec);
public:
	TSharedPtr<FCombatAbilityActorInfo>	CombatAbilityActorInfo;
	TMap<FGameplayTag, FGameplayEventMulticastDelegate> GenericGameplayEventCallbacks;
	FGameplayTagContainer BlockedTags;
protected:
	TMap<FGameplayTag, TArray<FCombatAbilitySpecHandle>> GameplayEventTriggeredAbilities;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "CombatSystem|Abilities")
	FCombatAbilitySpecContainer ActivatableAbilities;

	// Handles to abilities that had their input pressed this frame.
	TArray<FCombatAbilitySpecHandle> InputPressedSpecHandles;
	// Handles to abilities that had their input released this frame.
	TArray<FCombatAbilitySpecHandle> InputReleasedSpecHandles;
	// Handles to abilities that have their input held.
	TArray<FCombatAbilitySpecHandle> InputHeldSpecHandles;
	
	UPROPERTY(VisibleAnywhere, Category = "Animations With Tag")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> AnimMontagesByTag;
private:
	


};
