// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/CombatAbility_MeleeWeapon.h"

#include "Actors/CombatSystem_WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UCombatAbility_MeleeWeapon::OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::OnGiveAbility(Handle, ActorInfo);
	AttachWeaponToCharacter(ActorInfo);
}

void UCombatAbility_MeleeWeapon::AttachWeaponToCharacter(const FCombatAbilityActorInfo* ActorInfo)
{
	if (!WeaponClass || !ActorInfo->AvatarActor.Get()) return;
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = ActorInfo->AvatarActor.Get();
	
	ACombatSystem_WeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<ACombatSystem_WeaponBase>(WeaponClass,ActorSpawnParameters);
	SpawnedWeapon->AttachToComponent(ActorInfo->SkeletalMeshComponent.Get(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),SocketToAttachTo);
}
