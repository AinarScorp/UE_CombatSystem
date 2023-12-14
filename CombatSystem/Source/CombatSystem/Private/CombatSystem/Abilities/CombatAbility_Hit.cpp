// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/CombatAbility_Hit.h"

#include "Kismet/GameplayStatics.h"
#include "Library/EinarGameplayLibrary.h"

void UCombatAbility_Hit::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	if (!TriggerEventData || !TriggerEventData->Target)
	{
		EndAbility(Handle,ActorInfo,true);
	}
	Super::ActivateAbility(Handle, ActorInfo, TriggerEventData);
	const FHitMontageWithSection MontageWithSection = GetSectionNameFromContext(TriggerEventData);
	ActorInfo->CombatSystemComponent->PlayMontage(this,MontageWithSection.HitMontage,1,MontageWithSection.SectionName);
	EndAbility(Handle,ActorInfo,false);
}



FHitMontageWithSection UCombatAbility_Hit::GetSectionNameFromContext(const FCombatEventData* TriggerEventData) const
{
	switch (UEinarGameplayLibrary::GetActorRelativeContextSingle(TriggerEventData->Target,TriggerEventData->HitResult.ImpactPoint))
	{
	case ERelativeContext::Behind:
		return HitFromBehindMontage;
	case ERelativeContext::InFront:
		return HitFromFrontMontage;
	case ERelativeContext::ToTheRight:
		return HitFromRightMontage;
	case ERelativeContext::ToTheLeft:
		return HitFromLeftMontage;
	default: return FHitMontageWithSection();
	}
}
