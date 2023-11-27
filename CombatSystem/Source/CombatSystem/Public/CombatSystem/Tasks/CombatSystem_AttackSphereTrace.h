// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem_AttackTrace.h"
#include "Helpers/HelperStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatSystem_AttackSphereTrace.generated.h"

class UCombatSystem_AbilityComponent;
class USkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_AttackSphereTrace : public UCombatSystem_AttackTrace
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "CombatSystem|Ability", meta = (DefaultToSelf = "OwningActor"))
	static UCombatSystem_AttackSphereTrace* AttackSphereTrace(AActor* OwningActor, USkeletalMeshComponent* SkeletalMeshComponent,FName WeaponCenterSocketName,float Radius, ETraceTypeQuery TraceChannel, TArray<AActor*> InActorsToIgnore);
	virtual void TickTask(float DeltaTime) override;

private:
	float Radius;


};
