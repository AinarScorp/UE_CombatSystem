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
	virtual void InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo) override;
protected:
	virtual void StartNextAttack();
	// UFUNCTION()
	// virtual void MontageStoppedPlaying();
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
	TWeakObjectPtr<UCombatSystem_PlayMontage> MontageTask;
	int CurrentComboIndex = 0;
	bool ListeningToInput;
	
};
