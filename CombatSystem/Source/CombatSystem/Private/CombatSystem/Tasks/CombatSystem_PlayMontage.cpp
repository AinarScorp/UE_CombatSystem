// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"

#include "CombatSystem/Abilities/CombatAbility.h"
#include "Components/Actor/CombatSystemComponent.h"
#include "Interfaces/CombatSystem_AbilityInterface.h"

void UCombatSystem_PlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted && Ability)
	{
		OnCompleted.Broadcast();
	}

	EndTask();
}


void UCombatSystem_PlayMontage::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!Ability) return;

	if (Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			if (UCombatSystemComponent* CSC = CombatSystemComponent.Get())
			{
				CSC->ClearAnimatingAbility(Ability);
			}
		}
	}

	if (bInterrupted)
	{
		OnInterrupted.Broadcast();
	}
	else
	{
		OnBlendOut.Broadcast();
	}
}

void UCombatSystem_PlayMontage::OnMontageInterrupted()
{
	if (StopPlayingMontage() && Ability)
	{
		OnInterrupted.Broadcast();
	}
}


UCombatSystem_PlayMontage* UCombatSystem_PlayMontage::CreatePlayMontageProxy(UCombatAbility* OwningAbility, const FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds)
{
	UCombatSystem_PlayMontage* MyObj = NewTask<UCombatSystem_PlayMontage>(OwningAbility, TaskInstanceName);
	MyObj->Ability = OwningAbility;
	MyObj->CombatSystemComponent = OwningAbility->GetCurrentActorInfo()->CombatSystemComponent;
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	//MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	return MyObj;
}

void UCombatSystem_PlayMontage::Activate()
{
	if (!Ability) return;

	bool bPlayedMontage = false;

	if (UCombatSystemComponent* CSC = CombatSystemComponent.Get())
	{
		const FCombatAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->AnimInstance.Get();
		if (!AnimInstance) return;

		if (CSC->PlayMontage(Ability, MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
		{
			InterruptedHandle = Ability->OnCombatAbilityCancelled.AddUObject(this, &UCombatSystem_PlayMontage::OnMontageInterrupted);

			BlendingOutDelegate.BindUObject(this, &UCombatSystem_PlayMontage::OnMontageBlendingOut);
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

			MontageEndedDelegate.BindUObject(this, &UCombatSystem_PlayMontage::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

			bPlayedMontage = true;
		}
	}


	if (!bPlayedMontage)
	{
		OnCancelled.Broadcast();
	}
}

void UCombatSystem_PlayMontage::ExternalCancel()
{
	if (Ability)
	{
		OnCancelled.Broadcast();
	}
	Super::ExternalCancel();
}


void UCombatSystem_PlayMontage::OnDestroy(bool bInOwnerFinished)
{
	if (Ability)
	{
		Ability->OnCombatAbilityCancelled.Remove(InterruptedHandle);
		if (bInOwnerFinished && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}
	Super::OnDestroy(bInOwnerFinished);
}

bool UCombatSystem_PlayMontage::StopPlayingMontage()
{
	if (Ability == nullptr)return false;

	const FCombatAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr) return false;

	const UAnimInstance* AnimInstance = ActorInfo->AnimInstance.Get();
	if (AnimInstance == nullptr) return false;

	UCombatSystemComponent* CSC = CombatSystemComponent.Get();
	if (!CSC || !Ability) return false;

	if (CSC->GetAnimatingAbility() != Ability || CSC->GetCurrentMontage() != MontageToPlay) return false;

	// Unbind delegates so they don't get called as well
	FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
	if (MontageInstance)
	{
		MontageInstance->OnMontageBlendingOutStarted.Unbind();
		MontageInstance->OnMontageEnded.Unbind();
	}

	CSC->CurrentMontageStop();
	return true;
}

