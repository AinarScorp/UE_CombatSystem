// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"

#include "CombatSystem/CombatAbility.h"
#include "Components/Actor/CombatSystem_AbilityComponent.h"
#include "Interfaces/CombatSystem_AbilityInterface.h"

void UCombatSystem_PlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast();
		}
	}

	EndTask();
}

UCombatSystem_PlayMontage* UCombatSystem_PlayMontage::CreatePlayMontageProxy(UCombatAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection,bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds)
{
	UCombatSystem_PlayMontage* MyObj = NewTask<UCombatSystem_PlayMontage>(*OwningAbility->GetCurrentActorInfo()->AvatarActor, TaskInstanceName);
	MyObj->Ability = OwningAbility;
	MyObj->CombatSystemComponent = OwningAbility->GetCurrentActorInfo()->CombatAbilitySystemComponent;

	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	return MyObj;
}

void UCombatSystem_PlayMontage::Activate()
{
	if (!Ability) return;

	bool bPlayedMontage = false;

	if (UCombatSystem_AbilityComponent* CSC = CombatSystemComponent.Get())
	{
		const FCombatAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->SkeletalMeshComponent->GetAnimInstance();
		if (!AnimInstance) return;

		if (CSC->PlayMontage(Ability, MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
		{
			// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
			// if (ShouldBroadcastAbilityTaskDelegates() == false)
			// {
			// 	return;
			// }


			MontageEndedDelegate.BindUObject(this, &UCombatSystem_PlayMontage::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
			bPlayedMontage = true;
		}
	}


	if (!bPlayedMontage)
	{
		// if (ShouldBroadcastAbilityTaskDelegates())
		// {
		// 	OnCancelled.Broadcast();
		// }
	}
}


bool UCombatSystem_PlayMontage::StopPlayingMontage()
{
	if (Ability == nullptr)
	{
		return false;
	}

	const FCombatAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->SkeletalMeshComponent->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UCombatSystem_AbilityComponent* CSC = CombatSystemComponent.Get();
	// if (CSC && Ability)
	// {
	// 	if (CSC->GetAnimatingAbility() == Ability && CSC->GetCurrentMontage() == MontageToPlay)
	// 	{
	// 		// Unbind delegates so they don't get called as well
	// 		FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
	// 		if (MontageInstance)
	// 		{
	// 			MontageInstance->OnMontageBlendingOutStarted.Unbind();
	// 			MontageInstance->OnMontageEnded.Unbind();
	// 		}
	//
	// 		CSC->CurrentMontageStop();
	// 		return true;
	// 	}
	// }

	return false;
}

bool UCombatSystem_PlayMontage::ShouldBroadcastAbilityTaskDelegates() const
{
	//bool ShouldBroadcast = (Ability);
	return Ability != nullptr;
}
