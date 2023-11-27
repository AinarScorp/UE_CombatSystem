// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "CombatAbility_Combo.generated.h"

USTRUCT(BlueprintType)
struct FComboAnimInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimMontage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName AnimSection;
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
	virtual void InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo) override;
	virtual void EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled) override;
protected:
	virtual void StartNextAttack();
	UFUNCTION()
	virtual void MontageStoppedPlaying();
	UFUNCTION()
	virtual void AttackInputWindowStarted(FCombatEventData Payload);
	UFUNCTION()
	virtual void AttackInputWindowEnded(FCombatEventData Payload);
private:
	void RotateToMoveInput() const;
	void StartNewWaitTaskForInputWindowStart();
	void StartNewWaitTaskForInputWindowEnd();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FComboAnimInfo> Combos;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bShouldRotateToInput = true;
private:
	TWeakObjectPtr<UCombatSystem_WaitGameplayEvent> WaitForInputWindowStartTask;
	TWeakObjectPtr<UCombatSystem_WaitGameplayEvent> WaitForInputWindowEndTask;
	int CurrentComboIndex = 0;
	bool ListeningToInput;
	
};
