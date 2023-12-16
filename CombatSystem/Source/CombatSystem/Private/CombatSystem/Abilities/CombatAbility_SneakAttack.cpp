// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/CombatAbility_SneakAttack.h"

#include "CombatSystem/Library/CombatSystem_AbilityLibrary.h"
#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Library/EinarGameplayLibrary.h"
#include "Library/EinarMathLibrary.h"
#include "Tags/CombatSystem_GameplayTags.h"

bool UCombatAbility_SneakAttack::CanActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags))
	{
		return false;
	}
	AActor* Avatar = ActorInfo->AvatarActor.Get();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Avatar);
	TArray<FHitResult> HitResults;
	bool HitSomeone = UEinarMathLibrary::ConeCastByChannel(Avatar, Avatar->GetActorLocation(), Avatar->GetActorForwardVector(), FVector::UpVector, SneakAttackRadius, ConeCastRayCount, SneakAttackVisionAngle, EnemyChannel, ActorsToIgnore, TraceDebugInfo, HitResults);
	if (!HitSomeone)
	{
		return false;
	}
	AActor* ClosestTarget = nullptr;
	float ShortestDistance = 99999999;
	for (auto HitResult : HitResults)
	{
		if (ClosestTarget == HitResult.GetActor())
		{
			continue;
		}
		float Distance = FVector::Distance(HitResult.GetActor()->GetActorLocation(), Avatar->GetActorLocation());
		if (Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			ClosestTarget = HitResult.GetActor();
		}
	}
	if (!ClosestTarget)
	{
		return false;
	}
	const int32 RelativeContext = UEinarGameplayLibrary::GetRelativeContext(ClosestTarget, Avatar);
	if (!UEinarGameplayLibrary::ContextPredicate(RelativeContext, Context))
	{
		return false;
	}

	Target = ClosestTarget;
	return true;
}

void UCombatAbility_SneakAttack::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, TriggerEventData);
	UCombatSystem_PlayMontage* MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, "AttackCombo", StealthKillMontage, 1);
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbility_SneakAttack::InternalEndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UCombatAbility_SneakAttack::InternalEndAbility);
	
	MontageTask->ReadyForActivation();
	UCombatSystem_AbilityLibrary::SendGameplayEventToActor(Target,FCombatSystem_GameplayTags::Get().Hit_Sneak);
}

void UCombatAbility_SneakAttack::EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, bWasCancelled);
	Target = nullptr;
}
