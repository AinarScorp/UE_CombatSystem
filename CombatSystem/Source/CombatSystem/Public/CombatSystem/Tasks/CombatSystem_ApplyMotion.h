// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "CombatSystem_ApplyMotion.generated.h"

class UCombatAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FApplyMotionSimpleDelegate);

/**
 * 
 */
//TODO: if controlled manually is false then I should make things for a timer that will make this stop automatically
UCLASS()
class COMBATSYSTEM_API UCombatSystem_ApplyMotion : public UGameplayTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "CombatAbility|Tasks", meta = (DisplayName = "ApplyMotion"))
	static UCombatSystem_ApplyMotion* CreateApplyMotionTask(UCombatAbility* OwningAbility, FName TaskInstanceName, FVector MotionDirection, float MotionSpeed,bool ControlManually= false, AActor* ActorToMove = nullptr);

	UCombatSystem_ApplyMotion(const FObjectInitializer& ObjectInitializer);
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void ExternalCancel() override;
protected:
	virtual void OnDestroy(bool bInOwnerFinished) override;
public:
	FApplyMotionSimpleDelegate OnCompleted;
	FApplyMotionSimpleDelegate OnCancelled;
private:
	UPROPERTY()
	TObjectPtr<UCombatAbility> Ability;
	TWeakObjectPtr<AActor> ActorToMove;
	FVector MotionDirection{0};
	float MotionSpeed = 0;
	bool ControlManually = false;
};
