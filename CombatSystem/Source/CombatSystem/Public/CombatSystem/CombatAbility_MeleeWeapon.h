// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/CombatAbility.h"
#include "CombatAbility_MeleeWeapon.generated.h"

class ACombatSystem_WeaponBase;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class COMBATSYSTEM_API UCombatAbility_MeleeWeapon : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void OnGiveAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo) override;
private:
	void AttachWeaponToCharacter(const FCombatAbilityActorInfo* ActorInfo);
private:
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon")
	TSubclassOf<ACombatSystem_WeaponBase> WeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon")
	TSubclassOf<AActor> WeaponClasssss;
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon")
	FName SocketToAttachTo =  "weapon_r_socket";
	//When activated you should call for attack sphere trace and waitgameplayevent
};
