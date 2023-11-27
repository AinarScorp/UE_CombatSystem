// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "CombatSystem_AttackSphereTrace.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_AttackSphereTrace : public UGameplayTask
{
	GENERATED_BODY()
public:
	UCombatSystem_AttackSphereTrace(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability", meta = (DefaultToSelf = "OwningActor"))
	static UCombatSystem_AttackSphereTrace* AttackSphereTrace(AActor* OwningActor);
	virtual void TickTask(float DeltaTime) override;
	virtual void Activate() override;
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitActor, AActor*, HitActor, FHitResult, HitResult);

};
