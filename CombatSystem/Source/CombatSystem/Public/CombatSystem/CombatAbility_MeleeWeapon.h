// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/CombatAbility.h"
#include "Helpers/HelperStructs.h"
#include "CombatAbility_MeleeWeapon.generated.h"

class UCombatSystem_AttackBoxTrace;
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
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData = nullptr) override;
	virtual void EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled) override;
protected:
	UFUNCTION()
	virtual void OnHitTarget(AActor* ThisActor, FHitResult HitResult);
	UFUNCTION()
	void FinishExecute(FCombatEventData Payload);
private:
	void AttachWeaponToCharacter(const FCombatAbilityActorInfo* ActorInfo);
private:
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon")
	TSubclassOf<ACombatSystem_WeaponBase> WeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon")
	FName SocketToAttachTo =  "weapon_r_socket";
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon|AttackExecution")
	FName WeaponCenterSocket =  "WeaponCenter";
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon|AttackExecution")
	FVector AttackBoxHalfSize {10};
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon|AttackExecution")
	FRotator AttackRotationOffset;
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon|AttackExecution")
	TEnumAsByte<ETraceTypeQuery> AttackChannel;
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon|AttackExecution|Debug")
	FTraceDebugInfo TraceDebugInfo;

	TWeakObjectPtr<UCombatSystem_AttackBoxTrace> AttackBoxTrace;
	TWeakObjectPtr<ACombatSystem_WeaponBase> Weapon;
	
	//When activated you should call for attack sphere trace and waitgameplayevent
};
