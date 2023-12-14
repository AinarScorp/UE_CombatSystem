// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "CombatSystem_PlayMontage.generated.h"
class UCombatAbility;
class UCombatSystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMontageSimpleDelegate);

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

	virtual void Activate() override;
	virtual void ExternalCancel() override;
	
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void OnDestroy(bool bInOwnerFinished) override;
	bool StopPlayingMontage();
public:
	UPROPERTY(BlueprintAssignable)
	FMontageSimpleDelegate	OnCompleted;
	UPROPERTY(BlueprintAssignable)
	FMontageSimpleDelegate	OnBlendOut;
	UPROPERTY(BlueprintAssignable)
	FMontageSimpleDelegate	OnInterrupted;
	UPROPERTY(BlueprintAssignable)
	FMontageSimpleDelegate	OnCancelled;
protected:
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;
private:

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
	TWeakObjectPtr<UCombatSystemComponent> CombatSystemComponent;
	UPROPERTY()
	TObjectPtr<UCombatAbility> Ability;
};
