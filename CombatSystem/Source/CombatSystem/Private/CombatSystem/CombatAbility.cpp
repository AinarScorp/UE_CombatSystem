// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/CombatAbility.h"

#include "CombatSystem/Structs/CombatAbilityActorInfo.h"
#include "CombatSystem/Structs/CombatAbilitySpecHandle.h"
#include "Components/Actor/CombatSystem_AbilityComponent.h"


UCombatAbility::UCombatAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	auto ImplementedInBlueprint = [](const UFunction* Func) -> bool
	{
		return Func && ensure(Func->GetOuter())
			&& Func->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
	};
	static FName FuncName = FName(TEXT("BP_CanActivateAbility"));
	UFunction* CanActivateFunction = GetClass()->FindFunctionByName(FuncName);
	bHasBlueprintCanUse = ImplementedInBlueprint(CanActivateFunction);
}

bool UCombatAbility::CanActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	AActor* const AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	if (AvatarActor == nullptr)
	{
		return false;
	}

	static FGameplayTagContainer DummyContainer;
	DummyContainer.Reset();


	// make sure the ability system component is valid, if not bail out.
	UCombatSystem_AbilityComponent* const AbilitySystemComponent = ActorInfo->CombatAbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
	{
		return false;
	}
	if (!DoesAbilitySatisfyTagRequirements(*AbilitySystemComponent, SourceTags, TargetTags))
	{
		return false;
	}

	if (bHasBlueprintCanUse)
	{
		UE_LOG(LogTemp,Display, TEXT("%s I am in blueprints"), *GetName())
		return BP_CanActivateAbility(*ActorInfo);
	}
	return true;
}

bool UCombatAbility::DoesAbilitySatisfyTagRequirements(const UCombatSystem_AbilityComponent& CombatAbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	// Check if any of this ability's tags are currently blocked
	if (CombatAbilitySystemComponent.AreAbilityTagsBlocked(AbilityTags))
	{
		return false;
	}

	// // Check to see the required/blocked tags for this ability
	// if (ActivationBlockedTags.Num() || ActivationRequiredTags.Num())
	// {
	// 	static FGameplayTagContainer AbilitySystemComponentTags;
	// 	AbilitySystemComponentTags.Reset();
	//
	// 	AbilitySystemComponent.GetOwnedGameplayTags(AbilitySystemComponentTags);
	//
	// 	if (AbilitySystemComponentTags.HasAny(ActivationBlockedTags))
	// 	{
	// 		bBlocked = true;
	// 	}
	//
	// 	if (!AbilitySystemComponentTags.HasAll(ActivationRequiredTags))
	// 	{
	// 		bMissing = true;
	// 	}
	// }
	if (SourceTags != nullptr)
	{
		//if (SourceBlockedTags.Num() || SourceRequiredTags.Num())
		if (SourceBlockedTags.Num())
		{
			if (SourceTags->HasAny(SourceBlockedTags))
			{
				return false;
			}

			// if (!SourceTags->HasAll(SourceRequiredTags))
			// {
			// 	return false;
			// 	bMissing = true;
			// }
		}
	}

	if (TargetTags != nullptr)
	{
		//if (TargetBlockedTags.Num() || TargetRequiredTags.Num())
		if (TargetBlockedTags.Num())
		{
			if (TargetTags->HasAny(TargetBlockedTags))
			{
				return false;
			}
			//
			// if (!TargetTags->HasAll(TargetRequiredTags))
			// {
			// 	return false;
			// }
		}
	}

	return true;
}

bool UCombatAbility::CheckCooldown(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	// const FGameplayTagContainer* CooldownTags = GetCooldownTags();
	// if (CooldownTags)
	// {
	// 	if (CooldownTags->Num() > 0)
	// 	{
	// 		UCombatSystem_AbilityComponent* const AbilitySystemComponent = ActorInfo->CombatAbilitySystemComponent.Get();
	// 		check(AbilitySystemComponent != nullptr);
	// 		if (AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags))
	// 		{
	// 			const FGameplayTag& CooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;
	//
	// 			if (OptionalRelevantTags && CooldownTag.IsValid())
	// 			{
	// 				OptionalRelevantTags->AddTag(CooldownTag);
	// 			}
	//
	// 			return false;
	// 		}
	// 	}
	// }
	return true;
}

bool UCombatAbility::CheckCost(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	// UGameplayEffect* CostGE = GetCostGameplayEffect();
	// if (CostGE)
	// {
	// 	UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	// 	check(AbilitySystemComponent != nullptr);
	// 	if (!AbilitySystemComponent->CanApplyAttributeModifiers(CostGE, GetAbilityLevel(Handle, ActorInfo), MakeEffectContext(Handle, ActorInfo)))
	// 	{
	// 		const FGameplayTag& CostTag = UAbilitySystemGlobals::Get().ActivateFailCostTag;
	//
	// 		if (OptionalRelevantTags && CostTag.IsValid())
	// 		{
	// 			OptionalRelevantTags->AddTag(CostTag);
	// 		}
	// 		return false;
	// 	}
	// }
	return true;
}

void UCombatAbility::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	if (TriggerEventData)
	{
		CombatEventData = *TriggerEventData;
	}
	UCombatSystem_AbilityComponent* Comp = ActorInfo->CombatAbilitySystemComponent.Get();
	FCombatAbilitySpec* Spec = Comp->FindAbilitySpecFromHandle(Handle);

	Comp->ApplyAbilityBlockAndCancelTags(AbilityTags, this, true, BlockAbilitiesWithTag, true, CancelAbilitiesWithTag);
	Comp->ApplyAbilityContainedTags(OnGiveAbilityGrandTags, false);
	Spec->ActiveCount++;
	bIsActive = true;
	BP_ActivateAbility();
}

void UCombatAbility::EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled)
{
	UCombatSystem_AbilityComponent* Comp = ActorInfo->CombatAbilitySystemComponent.Get();
	FCombatAbilitySpec* Spec = Comp->FindAbilitySpecFromHandle(Handle);
	Comp->ApplyAbilityBlockAndCancelTags(AbilityTags, this, false, BlockAbilitiesWithTag, false, CancelAbilitiesWithTag);
	Comp->ApplyAbilityContainedTags(OnGiveAbilityGrandTags, true);

	Spec->ActiveCount--;
	bIsActive = false;
	Comp->NotifyAbilityEnded(Handle, this, bWasCancelled);
	BP_OnEndAbility(bWasCancelled);
}

void UCombatAbility::CancelAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	if (OnCombatAbilityCancelled.IsBound())
	{
		OnCombatAbilityCancelled.Broadcast();
	}
	if (bIsCancelable)
	{
		EndAbility(Handle, ActorInfo, true);
	}
}

void UCombatAbility::OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	CurrentActorInfo = ActorInfo;
	CurrentSpecHandle = Handle;
}

void UCombatAbility::BP_EndAbility()
{
	InternalEndAbility();
}

void UCombatAbility::InternalEndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, false);
}

void UCombatAbility::InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	BP_OnInputPressed();
}

void UCombatAbility::InputReleased(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	BP_OnInputReleased();
}
