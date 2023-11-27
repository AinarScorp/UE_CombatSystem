// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/CombatAbility_MeleeWeapon.h"

#include "Actors/CombatSystem_WeaponBase.h"
#include "CombatSystem/Library/CombatSystem_AbilityLibrary.h"
#include "CombatSystem/Tasks/CombatSystem_AttackBoxTrace.h"
#include "CombatSystem/Tasks/CombatSystem_AttackSphereTrace.h"
#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"
#include "Kismet/GameplayStatics.h"
#include "Tags/CombatSystem_GameplayTags.h"

void UCombatAbility_MeleeWeapon::OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::OnGiveAbility(Handle, ActorInfo);
	AttachWeaponToCharacter(ActorInfo);
}

void UCombatAbility_MeleeWeapon::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, TriggerEventData);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(ActorInfo->AvatarActor.Get());
	AttackBoxTrace = UCombatSystem_AttackBoxTrace::AttackBoxTrace(ActorInfo->AvatarActor.Get(),Weapon->GetWeaponMesh(),WeaponCenterSocket,AttackBoxHalfSize,AttackRotationOffset,AttackChannel,ActorsToIgnore);
	AttackBoxTrace->SetupDebug(TraceDebugInfo);
	AttackBoxTrace->OnHitTarget.AddDynamic(this,&UCombatAbility_MeleeWeapon::OnHitTarget);
	AttackBoxTrace->ReadyForActivation();

	UCombatSystem_WaitGameplayEvent* WaitTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(CurrentActorInfo->AvatarActor.Get(),FCombatSystem_GameplayTags::Get().Weapon_FinishExecute,true);
	WaitTask->EventReceived.AddDynamic(this, &UCombatAbility_MeleeWeapon::FinishExecute);
	WaitTask->ReadyForActivation();
}

void UCombatAbility_MeleeWeapon::EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, bWasCancelled);
	if (AttackBoxTrace.IsValid())
	{
		AttackBoxTrace->EndTask();
	}
}

void UCombatAbility_MeleeWeapon::OnHitTarget(AActor* ThisActor, FHitResult HitResult)
{
	FCombatEventData EventData;
	EventData.Instigator = ThisActor;
	EventData.Target = HitResult.GetActor();
	EventData.EventTag  = FCombatSystem_GameplayTags::Get().Weapon;
	EventData.HitResult = HitResult;
	UCombatSystem_AbilityLibrary::SendGameplayEventToActor(HitResult.GetActor(),FCombatSystem_GameplayTags::Get().Hit, EventData);
}

void UCombatAbility_MeleeWeapon::FinishExecute(FCombatEventData Payload)
{
	InternalEndAbility();
}

void UCombatAbility_MeleeWeapon::AttachWeaponToCharacter(const FCombatAbilityActorInfo* ActorInfo)
{
	if (!WeaponClass || !ActorInfo->AvatarActor.Get()) return;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = ActorInfo->AvatarActor.Get();
	
	Weapon = GetWorld()->SpawnActor<ACombatSystem_WeaponBase>(WeaponClass,ActorSpawnParameters);
	Weapon->AttachToComponent(ActorInfo->SkeletalMeshComponent.Get(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),SocketToAttachTo);
}
