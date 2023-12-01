// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "CombatSystem_PlayMontage.generated.h"
class UCombatAbility;
class UCombatSystem_AbilityComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMontageWaitSimpleDelegate);

/**
 * 
 */
//TODO: Add on interrupted 
UCLASS()
class COMBATSYSTEM_API UCombatSystem_PlayMontage : public UGameplayTask
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="PlayMontage", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCombatSystem_PlayMontage* CreatePlayMontageProxy(UCombatAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnMontageInterrupted();

	virtual void Activate() override;
	
	bool ShouldBroadcastAbilityTaskDelegates() const;
public:
	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate	OnCompleted;
	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate	OnInterrupted;
protected:
	virtual void OnDestroy(bool bInOwnerFinished) override;

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	float StartTimeSeconds;

	UPROPERTY()
	bool bStopWhenAbilityEnds;
private:
	TWeakObjectPtr<UCombatSystem_AbilityComponent> CombatSystemComponent;
	UPROPERTY()
	TObjectPtr<UCombatAbility> Ability;
};