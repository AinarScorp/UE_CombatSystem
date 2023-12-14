// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/CombatAbility.h"

#include "CombatSystem/Structs/CombatAbilityActorInfo.h"
#include "CombatSystem/Structs/CombatAbilitySpecHandle.h"
#include "Components/Actor/CombatSystemComponent.h"


UCombatAbility::UCombatAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	auto ImplementedInBlueprint = [](const UFunction* Func) -> bool
	{
		return Func && ensure(Func->GetOuter())
			&& Func->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
	};
	static FName FuncName = FName(TEXT("BP_CanActivateAbility"));
	UFunction* CanActivateFunction = GetClass()->FindFunctionByName(FuncName);
	bCanActivateAbilityImplementedInBP = ImplementedInBlueprint(CanActivateFunction);
}

UGameplayTasksComponent* UCombatAbility::GetGameplayTasksComponent(const UGameplayTask& Task) const
{
	return GetCurrentActorInfo() ? GetCurrentActorInfo()->CombatSystemComponent.Get() : nullptr;
}

AActor* UCombatAbility::GetGameplayTaskAvatar(const UGameplayTask* Task) const
{
	return GetCurrentActorInfo()->AvatarActor.Get();
}

AActor* UCombatAbility::GetGameplayTaskOwner(const UGameplayTask* Task) const
{
	return GetCurrentActorInfo()->OwnerActor.Get();
}

void UCombatAbility::OnGameplayTaskActivated(UGameplayTask& Task)
{
	ActiveTasks.Add(&Task);
}

void UCombatAbility::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	ActiveTasks.Remove(&Task);
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
	UCombatSystemComponent* const AbilitySystemComponent = ActorInfo->CombatSystemComponent.Get();
	if (!AbilitySystemComponent)
	{
		return false;
	}
	if (!DoesAbilitySatisfyTagRequirements(*AbilitySystemComponent, SourceTags, TargetTags))
	{
		return false;
	}

	if (bCanActivateAbilityImplementedInBP)
	{
		UE_LOG(LogTemp, Display, TEXT("%s I am in blueprints"), *GetName())
		return BP_CanActivateAbility(*ActorInfo);
	}
	return true;
}

bool UCombatAbility::DoesAbilitySatisfyTagRequirements(const UCombatSystemComponent& CombatAbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	if (CombatAbilitySystemComponent.AreAbilityTagsBlocked(AbilityTags))
	{
		return false;
	}

	if (SourceTags != nullptr)
	{
		if (SourceBlockedTags.Num())
		{
			if (SourceTags->HasAny(SourceBlockedTags))
			{
				return false;
			}
		}
	}

	if (TargetTags != nullptr)
	{
		if (TargetBlockedTags.Num())
		{
			if (TargetTags->HasAny(TargetBlockedTags))
			{
				return false;
			}
		}
	}

	return true;
}


void UCombatAbility::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	if (TriggerEventData)
	{
		CombatEventData = *TriggerEventData;
	}
	UCombatSystemComponent* Comp = ActorInfo->CombatSystemComponent.Get();
	FCombatAbilitySpec* Spec = Comp->FindAbilitySpecFromHandle(Handle);

	Comp->ApplyAbilityBlockAndCancelTags(AbilityTags, this, true, BlockAbilitiesWithTag, true, CancelAbilitiesWithTag);
	Comp->ApplyAbilityContainedTags(OnGiveAbilityGrandTags, false);
	Spec->ActiveCount++;
	bIsActive = true;
	BP_ActivateAbility();
}

void UCombatAbility::EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled)
{
	if (!bIsActive) return;

	UCombatSystemComponent* Comp = ActorInfo->CombatSystemComponent.Get();
	FCombatAbilitySpec* Spec = Comp->FindAbilitySpecFromHandle(Handle);

	Comp->ApplyAbilityBlockAndCancelTags(AbilityTags, this, false, BlockAbilitiesWithTag, false, CancelAbilitiesWithTag);
	Comp->ApplyAbilityContainedTags(OnGiveAbilityGrandTags, true);


	Spec->ActiveCount--;
	bIsActive = false;
	Comp->NotifyAbilityEnded(Handle, this, bWasCancelled);
	//Put this into a function
	for (int32 TaskIdx = ActiveTasks.Num() - 1; TaskIdx >= 0 && ActiveTasks.Num() > 0; --TaskIdx)
	{
		UGameplayTask* Task = ActiveTasks[TaskIdx];
		if (Task)
		{
			Task->TaskOwnerEnded();
		}
	}
	ActiveTasks.Reset();
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

UAnimMontage* UCombatAbility::GetCurrentMontage() const
{
	return CurrentMontage;
}

void UCombatAbility::SetCurrentMontage(UAnimMontage* InCurrentMontage)
{
	CurrentMontage = InCurrentMontage;
}

void UCombatAbility::InternalEndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, false);
}

void UCombatAbility::InternalCancelAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, true);
}


void UCombatAbility::InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	BP_OnInputPressed();
}

void UCombatAbility::InputReleased(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	BP_OnInputReleased();
}
