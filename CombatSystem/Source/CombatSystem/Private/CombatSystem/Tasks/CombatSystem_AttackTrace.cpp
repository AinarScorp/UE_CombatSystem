// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_AttackTrace.h"

UCombatSystem_AttackTrace::UCombatSystem_AttackTrace(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bTickingTask =true;
}

void UCombatSystem_AttackTrace::Activate()
{
	Super::Activate();
	if (!OwningActor.IsValid() || !SkeletalMesh.IsValid())
	{
		EndTask();
		return;
	}
	StartTraceLocation = SkeletalMesh.Get()->GetSocketLocation(WeaponCenterSocketName);

}

void UCombatSystem_AttackTrace::SetupDebug(EDrawDebugTrace::Type TraceDebug, FLinearColor InTraceColor, FLinearColor InTraceHitColor, float DrawTime)
{
	DebugInfo.DrawDebugTraceType = TraceDebug;
	DebugInfo.TraceColor = InTraceColor;
	DebugInfo.TraceHitColor = InTraceHitColor;
	DebugInfo.DrawTime = DrawTime;
}

void UCombatSystem_AttackTrace::SetupDebug(const FTraceDebugInfo& TraceDebugInfo)
{
	DebugInfo = TraceDebugInfo;

}
