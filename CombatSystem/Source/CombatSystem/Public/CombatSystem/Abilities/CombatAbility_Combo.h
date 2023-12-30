// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "Helpers/HelperStructs.h"
#include "CombatAbility_Combo.generated.h"

class UCombatSystem_PlayMontage;
//TODO: Combine with Hit animations
USTRUCT(BlueprintType)
struct FComboAnimInfo : public FMontageWithSection
{
	GENERATED_BODY()
	//This tag is going to be expected to be triggered for this animation to play
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TriggerTag;
	// UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	// TObjectPtr<UAnimMontage> AnimMontage;
	// UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	// FName AnimSection;
};
class UCombatSystem_WaitGameplayEvent;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class COMBATSYSTEM_API UCombatAbility_Combo : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData = nullptr) override;
	virtual void EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled) override;
protected:

	UFUNCTION()
	virtual void AttackInputWindowStarted(FCombatEventData Payload);
	UFUNCTION()
	virtual void AttackInputWindowEnded(FCombatEventData Payload);
	virtual void InternalEndAbility() override;
private:
	void RotateToMoveInput() const;
	void PlayAttackAnimation(const FMontageWithSection& AttackMontage);

	void TryPerformAttack();
	void PerformAttack(const FMontageWithSection& AttackMontage);
	void ActivateWaitTasks(bool WaitToStartListeningForInput);
	UFUNCTION()
	virtual void ReceivedInput(FCombatEventData Payload);
	UFUNCTION()
	virtual void ReceivedAnimation(FCombatEventData Payload);
	UFUNCTION()
	virtual void StartListeningToInput(FCombatEventData Payload);
	UFUNCTION()
	virtual void StopListeningToInput(FCombatEventData Payload);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ActivatedByInputTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMontageWithSection FirstAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FComboAnimInfo> Combos;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bShouldRotateToInput = true;
private:
	TWeakObjectPtr<UCombatSystem_WaitGameplayEvent> WaitForAnimationReadyTask;
	TWeakObjectPtr<UCombatSystem_WaitGameplayEvent> WaitForInputTask;

	TWeakObjectPtr<UCombatSystem_PlayMontage> MontageTask;
	FMontageWithSection* NextAttackAnimation =nullptr;

	bool ListeningToInput;
	bool bReceivedInput;


};
