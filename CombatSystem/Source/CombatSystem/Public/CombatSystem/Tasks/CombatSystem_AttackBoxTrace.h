// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem_AttackTrace.h"
#include "CombatSystem_AttackBoxTrace.generated.h"

class UCombatAbility;
/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_AttackBoxTrace : public UCombatSystem_AttackTrace
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability", meta = (DefaultToSelf = "OwningActor"))
	static UCombatSystem_AttackBoxTrace* AttackBoxTrace(UCombatAbility* OwningAbility,USkeletalMeshComponent* SkeletalMeshComponent,FName WeaponCenterSocketName,FVector HalfSize,FRotator RotationOffset ,ETraceTypeQuery TraceChannel, TArray<AActor*> InActorsToIgnore);
	virtual void TickTask(float DeltaTime) override;
	
private:
	FVector HalfSize;
	FRotator RotationOffset;
};
