// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Actor/CombatSystemComponent.h"

#include "GameplayTagContainer.h"
#include "CombatSystem/Abilities/CombatAbility.h"
#include "CombatSystem/Abilities/InputAbility.h"
#include "CombatSystem/Structs/CombatAbilityActorInfo.h"
#include "Interfaces/CombatSystem_AbilityInterface.h"

void UCombatSystemComponent::OnRegister()
{
	Super::OnRegister();
	if (!CombatAbilityActorInfo.IsValid())
	{
		CombatAbilityActorInfo = TSharedPtr<FCombatAbilityActorInfo>(new FCombatAbilityActorInfo());
	}
}


void UCombatSystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (const FCombatAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.Ability || !AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
		InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
	}
}

void UCombatSystemComponent::AbilityInputTagPressedWithValue(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (const FCombatAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.Ability || !AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		const UInputAbility* InputAbility = CastChecked<UInputAbility>(AbilitySpec.InstancedAbility);
		InputAbility->SetInputActionValue(InputActionValue);

		InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
		//InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
	}
}

void UCombatSystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (const FCombatAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.Ability || !(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))) continue;

		InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
		InputHeldSpecHandles.Remove(AbilitySpec.Handle);
	}
}

void UCombatSystemComponent::AbilityInputTagReleasedWithValue(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (const FCombatAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!AbilitySpec.Ability || !(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))) continue;

		const UInputAbility* InputAbility = CastChecked<UInputAbility>(AbilitySpec.InstancedAbility);
		InputAbility->SetInputActionValue(InputActionValue);
		InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
		//InputHeldSpecHandles.Remove(AbilitySpec.Handle);
	}
}


void UCombatSystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FCombatAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (const FCombatAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FCombatAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				// if (LyraAbilityCDO->GetActivationPolicy() == ELyraAbilityActivationPolicy::WhileInputActive)
				AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
			}
		}
	}

	for (const FCombatAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FCombatAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FCombatAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}


	for (const FCombatAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FCombatAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (!AbilitySpec->Ability) continue;

			AbilitySpec->InputPressed = false;
			if (!AbilitySpec->IsActive()) continue;

			AbilitySpecInputReleased(*AbilitySpec);
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UCombatSystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	CombatAbilityActorInfo->InitFromActor(InOwnerActor, InAvatarActor, this);
}


float UCombatSystemComponent::PlayMontage(UCombatAbility* AnimatingAbility, UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName, float StartTimeSeconds)
{
	float Duration = -1.f;

	UAnimInstance* AnimInstance = CombatAbilityActorInfo.IsValid() ? CombatAbilityActorInfo->AnimInstance.Get() : nullptr;

	if (!AnimInstance || !NewAnimMontage) return Duration;

	Duration = AnimInstance->Montage_Play(NewAnimMontage, InPlayRate, EMontagePlayReturnType::MontageLength, StartTimeSeconds);
	if (Duration <= 0.f) return Duration;
	
	if (AnimatingAbility)
	{
		AnimatingAbility->SetCurrentMontage(NewAnimMontage);
	}
	AnimMontageInfo.AnimMontage = NewAnimMontage;
	AnimMontageInfo.AnimatingAbility = AnimatingAbility;
	if (StartSectionName != NAME_None)
	{
		AnimInstance->Montage_JumpToSection(StartSectionName, NewAnimMontage);
	}

	return Duration;
}

void UCombatSystemComponent::CurrentMontageStop(float OverrideBlendOutTime)
{
	UAnimInstance* AnimInstance = CombatAbilityActorInfo.IsValid() ? CombatAbilityActorInfo->SkeletalMeshComponent->GetAnimInstance() : nullptr;
	UAnimMontage* MontageToStop = AnimMontageInfo.AnimMontage;
	if (AnimInstance && MontageToStop && !AnimInstance->Montage_GetIsStopped(MontageToStop))
	{
		const float BlendOutTime = (OverrideBlendOutTime >= 0.0f ? OverrideBlendOutTime : MontageToStop->BlendOut.GetBlendTime());
		AnimInstance->Montage_Stop(BlendOutTime, MontageToStop);
	}
}

void UCombatSystemComponent::ClearAnimatingAbility(UCombatAbility* Ability)
{
	if (AnimMontageInfo.AnimatingAbility.Get() == Ability)
	{
		Ability->SetCurrentMontage(nullptr);
		AnimMontageInfo.AnimatingAbility = nullptr;
	}
}

UCombatAbility* UCombatSystemComponent::GetAnimatingAbility() const
{
	return AnimMontageInfo.AnimatingAbility.Get();
}

UAnimMontage* UCombatSystemComponent::GetCurrentMontage() const
{
	const UAnimInstance* AnimInstance = CombatAbilityActorInfo.IsValid() ? CombatAbilityActorInfo->SkeletalMeshComponent->GetAnimInstance() : nullptr;
	if (AnimMontageInfo.AnimMontage && AnimInstance && AnimInstance->Montage_IsActive(AnimMontageInfo.AnimMontage))
	{
		return AnimMontageInfo.AnimMontage;
	}

	return nullptr;
}



void UCombatSystemComponent::RegisterTriggerableAbilities(const FCombatAbilitySpec& AbilitySpec)
{
	for (const FCombatAbilityTriggerData& TriggerData : AbilitySpec.Ability->AbilityTriggers)
	{
		FGameplayTag EventTag = TriggerData.TriggerTag;

		//auto& TriggeredAbilityMap = (TriggerData.TriggerSource == ECombatAbilityTriggerSource::GameplayEvent) ? GameplayEventTriggeredAbilities : OwnedTagTriggeredAbilities;
		auto& TriggeredAbilityMap = (TriggerData.TriggerSource == ECombatAbilityTriggerSource::GameplayEvent) ? GameplayEventTriggeredAbilities : GameplayEventTriggeredAbilities;

		if (TriggeredAbilityMap.Contains(EventTag))
		{
			TriggeredAbilityMap[EventTag].AddUnique(AbilitySpec.Handle);
		}
		else
		{
			TArray<FCombatAbilitySpecHandle> Triggers;
			Triggers.Add(AbilitySpec.Handle);
			TriggeredAbilityMap.Add(EventTag, Triggers);
		}
	}
}

void UCombatSystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UCombatAbility* RequestingAbility, bool bEnableBlockTags,const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	//TODO: Check what happens if one thing removes tags, but there should be another one with the same
	if (bEnableBlockTags)
	{
		BlockedTags.AddTags(BlockTags);
	}
	else
	{
		BlockedTags.RemoveTags(BlockTags);
	}

	if (bExecuteCancelTags)
	{
		CancelAbilities(&CancelTags, nullptr, RequestingAbility);
	}
}

void UCombatSystemComponent::ApplyAbilityContainedTags(const FGameplayTagContainer& AbilityTags, bool bRemove)
{
	if (bRemove)
	{
		ContainedTags.RemoveTags(AbilityTags);
	}
	else
	{
		ContainedTags.AddTags(AbilityTags);
	}
}

void UCombatSystemComponent::CancelAbilities(const FGameplayTagContainer* WithTags, const FGameplayTagContainer* WithoutTags, UCombatAbility* Ignore)
{
	for (FCombatAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (!Spec.IsActive() || Spec.Ability == nullptr)
		{
			continue;
		}

		bool WithTagPass = (!WithTags || Spec.Ability->AbilityTags.HasAny(*WithTags));
		bool WithoutTagPass = (!WithoutTags || !Spec.Ability->AbilityTags.HasAny(*WithoutTags));

		if (WithTagPass && WithoutTagPass)
		{
			CancelAbilitySpec(Spec, Ignore);
		}
	}
}

void UCombatSystemComponent::CancelAbilitySpec(FCombatAbilitySpec& Spec, UCombatAbility* Ignore)
{
	FCombatAbilityActorInfo* ActorInfo = CombatAbilityActorInfo.Get();
	UCombatAbility* AbilityToCancel = Spec.InstancedAbility.Get();
	if (AbilityToCancel && Ignore != AbilityToCancel)
	{
		AbilityToCancel->CancelAbility(Spec.Handle, ActorInfo);
	}
}

FCombatAbilitySpec* UCombatSystemComponent::FindAbilitySpecFromHandle(FCombatAbilitySpecHandle Handle)
{
	for (FCombatAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Handle != Handle) continue;
		return &Spec;
	}
	return nullptr;
}

FCombatAbilitySpec* UCombatSystemComponent::FindAbilitySpecFromClass(const TSubclassOf<UCombatAbility> InAbilityClass)
{
	for (FCombatAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability->GetClass() == InAbilityClass)
		{
			return &Spec;
		}
	}

	return nullptr;
}

void UCombatSystemComponent::GetActivatableAbilitySpecsByAllMatchingTags(const FGameplayTagContainer& GameplayTagContainer, TArray<FCombatAbilitySpec*>& MatchingGameplayAbilities,bool bOnlyAbilitiesThatSatisfyTagRequirements) const
{
	if (!GameplayTagContainer.IsValid())
	{
		return;
	}

	for (const FCombatAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasAll(GameplayTagContainer))
		{
			if (!bOnlyAbilitiesThatSatisfyTagRequirements || Spec.Ability->DoesAbilitySatisfyTagRequirements(*this))
			{
				MatchingGameplayAbilities.Add(const_cast<FCombatAbilitySpec*>(&Spec));
			}
		}
	}
}

bool UCombatSystemComponent::AreAbilityTagsBlocked(const FGameplayTagContainer& Tags) const
{
	return Tags.HasAny(BlockedTags.GetTags());
}

bool UCombatSystemComponent::ContainsAbilityTags(const FGameplayTagContainer& Tags) const
{
	return Tags.HasAny(ContainedTags.GetTags());
}

bool UCombatSystemComponent::TryActivateAbility(FCombatAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation)
{
	FCombatAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityToActivate);
	if (!Spec) return false;
	if (Spec->PendingRemove || Spec->RemoveAfterActivation) return false;

	UCombatAbility* Ability = Spec->Ability;
	if (!Ability) return false;

	const FCombatAbilityActorInfo* ActorInfo = CombatAbilityActorInfo.Get();
	if (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid()) return false;
	
	return InternalTryActivateAbility(AbilityToActivate);
	
}

bool UCombatSystemComponent::TryActivateAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation)
{
	TArray<FCombatAbilitySpec*> AbilitiesToActivate;
	GetActivatableAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate);

	bool bSuccess = false;

	for (auto GameplayAbilitySpec : AbilitiesToActivate)
	{
		bSuccess |= TryActivateAbility(GameplayAbilitySpec->Handle, bAllowRemoteActivation);
	}
	return bSuccess;
}

bool UCombatSystemComponent::TryActivateAbilitiesByClass(TSubclassOf<UCombatAbility> InAbilityToActivate, bool bAllowRemoteActivation)
{
	bool bSuccess = false;

	const UCombatAbility* const InAbilityCDO = InAbilityToActivate.GetDefaultObject();

	for (const FCombatAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability == InAbilityCDO)
		{
			bSuccess |= TryActivateAbility(Spec.Handle, bAllowRemoteActivation);
			break;
		}
	}

	return bSuccess;
}

bool UCombatSystemComponent::TriggerAbilityFromGameplayEvent(FCombatAbilitySpecHandle AbilityToTrigger, FCombatAbilityActorInfo* ActorInfo, FGameplayTag Tag, const FCombatEventData* Payload,UCombatSystemComponent& Component)
{
	FCombatAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityToTrigger);

	const UCombatAbility* InstancedAbility = Spec->InstancedAbility;
	const UCombatAbility* Ability = InstancedAbility ? InstancedAbility : Spec->Ability;
	if (!ensure(Ability) || !ensure(Payload))
	{
		return false;
	}

	// Make a temp copy of the payload, and copy the event tag into it
	FCombatEventData TempEventData = *Payload;
	TempEventData.EventTag = Tag;

	return InternalTryActivateAbility(AbilityToTrigger, &TempEventData);
}


bool UCombatSystemComponent::InternalTryActivateAbility(FCombatAbilitySpecHandle Handle, const FCombatEventData* TriggerEventData)
{
	if (Handle.IsValid() == false)
	{
		return false;
	}

	FCombatAbilitySpec* Spec = FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		return false;
	}

	const FCombatAbilityActorInfo* ActorInfo = CombatAbilityActorInfo.Get();
	// make sure the ActorInfo and then Actor on that FGameplayAbilityActorInfo are valid, if not bail out.
	if (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	UCombatAbility* InstancedAbility = Spec->InstancedAbility;
	if (!InstancedAbility)
	{
		return false;
	}
	const FGameplayTagContainer* SourceTags = &ContainedTags.GetTags();
	const FGameplayTagContainer* TargetTags = nullptr;
	if (TriggerEventData != nullptr && TriggerEventData->Target)
	{
		const ICombatSystem_AbilityInterface* TargetAbilityInterface = Cast<ICombatSystem_AbilityInterface>(TriggerEventData->Target);
		if (TargetAbilityInterface && TargetAbilityInterface->GetCombatSystemComponent())
		{
			const FGameplayTagContainer TargetContainedTags = TargetAbilityInterface->GetCombatSystemComponent()->GetContainedAbilityTags();
			TargetTags = &TargetContainedTags;
		}
	}
	
	if (!InstancedAbility->CanActivateAbility(Handle, ActorInfo,SourceTags,TargetTags))
	{
		//NotifyAbilityFailed(Handle, CanActivateAbilitySource, InternalTryActivateAbilityFailureTags);
		return false;
	}
	if (Spec->IsActive())
	{
		if (InstancedAbility->bRetriggerInstancedAbility)
		{
			InstancedAbility->EndAbility(Handle, ActorInfo, false);
		}
		else
		{
			return false;
		}
	}

	//Ability->CallActivateAbility(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	InstancedAbility->ActivateAbility(Handle, ActorInfo,TriggerEventData);
	return true;
}

FCombatAbilitySpecHandle UCombatSystemComponent::GiveAbility(const FCombatAbilitySpec& AbilitySpec)
{
	if (!AbilitySpec.Ability)
	{
		return FCombatAbilitySpecHandle();
	}

	FCombatAbilitySpec& OwnedSpec = ActivatableAbilities.Items[ActivatableAbilities.Items.Add(AbilitySpec)];

	OwnedSpec.InstancedAbility = NewObject<UCombatAbility>(GetOwner(), OwnedSpec.Ability->GetClass());


	RegisterTriggerableAbilities(AbilitySpec);
	OwnedSpec.InstancedAbility->OnGiveAbility(AbilitySpec.Handle, CombatAbilityActorInfo.Get());
	return OwnedSpec.Handle;
}

FCombatAbilitySpecHandle UCombatSystemComponent::GiveAbility_BP(TSubclassOf<UCombatAbility> AbilityClass)
{
	const FCombatAbilitySpec AbilitySpec = FCombatAbilitySpec(AbilityClass);
	if (!IsValid(AbilitySpec.Ability))
	{
		return FCombatAbilitySpecHandle();
	}
	return GiveAbility(AbilitySpec);
}



void UCombatSystemComponent::AbilitySpecInputPressed(FCombatAbilitySpec& Spec) const
{
	Spec.InputPressed = true;
	if (Spec.IsActive())
	{
		Spec.InstancedAbility->InputPressed(Spec.Handle, CombatAbilityActorInfo.Get());
	}
}

void UCombatSystemComponent::AbilitySpecInputReleased(FCombatAbilitySpec& Spec) const
{
	Spec.InputPressed = false;
	if (Spec.IsActive())
	{
		Spec.InstancedAbility->InputReleased(Spec.Handle, CombatAbilityActorInfo.Get());
	}
}

int32 UCombatSystemComponent::HandleGameplayEvent(FGameplayTag EventTag, const FCombatEventData* Payload)
{
	int32 TriggeredCount = 0;
	FGameplayTag CurrentTag = EventTag;
	while (CurrentTag.IsValid())
	{
		if (GameplayEventTriggeredAbilities.Contains(CurrentTag))
		{
			TArray<FCombatAbilitySpecHandle> TriggeredAbilityHandles = GameplayEventTriggeredAbilities[CurrentTag];

			for (const FCombatAbilitySpecHandle& AbilityHandle : TriggeredAbilityHandles)
			{
				if (TriggerAbilityFromGameplayEvent(AbilityHandle, CombatAbilityActorInfo.Get(), EventTag, Payload, *this))
				{
					TriggeredCount++;
				}
			}
		}

		CurrentTag = CurrentTag.RequestDirectParent();
	}

	if (const FGameplayEventMulticastDelegate* Delegate = GenericGameplayEventCallbacks.Find(EventTag))
	{
		Delegate->Broadcast(Payload);
	}
	return TriggeredCount;
}

void UCombatSystemComponent::NotifyAbilityEnded(FCombatAbilitySpecHandle Handle, UCombatAbility* Ability, bool bWasCancelled)
{
	if (AnimMontageInfo.AnimatingAbility.Get() == Ability)
	{
		ClearAnimatingAbility(Ability);
	}
	AbilityEndedCallbacks.Broadcast(Ability);
	AbilityEndedCallbacks_BP.Broadcast(Ability);
}
