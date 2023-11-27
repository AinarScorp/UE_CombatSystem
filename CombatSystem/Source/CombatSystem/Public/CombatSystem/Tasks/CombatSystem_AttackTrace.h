// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "Helpers/HelperStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatSystem_AttackTrace.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBATSYSTEM_API UCombatSystem_AttackTrace : public UGameplayTask
{
	GENERATED_BODY()
public:
	UCombatSystem_AttackTrace(const FObjectInitializer& ObjectInitializer);
	virtual void Activate() override;
	
	void SetupDebug(EDrawDebugTrace::Type TraceDebug,FLinearColor InTraceColor, FLinearColor InTraceHitColor, float DrawTime);
	void SetupDebug(const FTraceDebugInfo& TraceDebugInfo);
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitTarget, AActor*, OwningActor, FHitResult, HitResult);
	UPROPERTY(BlueprintAssignable)
	FOnHitTarget OnHitTarget;
protected:
	TWeakObjectPtr<AActor> OwningActor;
	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMesh;
	FName WeaponCenterSocketName;
	FVector StartTraceLocation;
	FVector EndTraceLocation;
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	FTraceDebugInfo DebugInfo;
};
