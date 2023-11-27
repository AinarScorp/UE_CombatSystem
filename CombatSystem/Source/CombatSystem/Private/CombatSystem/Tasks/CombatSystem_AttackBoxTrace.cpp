// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_AttackBoxTrace.h"



UCombatSystem_AttackBoxTrace* UCombatSystem_AttackBoxTrace::AttackBoxTrace(AActor* OwningActor, USkeletalMeshComponent* SkeletalMeshComponent, const FName WeaponCenterSocketName, const FVector HalfSize, const FRotator RotationOffset, const ETraceTypeQuery TraceChannel, const TArray<AActor*> InActorsToIgnore)
{
	UCombatSystem_AttackBoxTrace* MyObj = NewTask<UCombatSystem_AttackBoxTrace>(OwningActor, "AttackBoxTrace");
	MyObj->OwningActor = OwningActor;
	MyObj->SkeletalMesh = SkeletalMeshComponent;
	MyObj->WeaponCenterSocketName = WeaponCenterSocketName;
	MyObj->HalfSize = HalfSize;
	MyObj->RotationOffset = RotationOffset;
	MyObj->TraceChannel = TraceChannel;
	MyObj->ActorsToIgnore = InActorsToIgnore;
	return MyObj;
}

void UCombatSystem_AttackBoxTrace::TickTask(float DeltaTime)
{
	EndTraceLocation = SkeletalMesh.Get()->GetSocketLocation(WeaponCenterSocketName);
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::BoxTraceMulti(OwningActor.Get(),StartTraceLocation,EndTraceLocation,HalfSize,SkeletalMesh->GetComponentRotation() + RotationOffset,TraceChannel, false, ActorsToIgnore,DebugInfo.DrawDebugTraceType,HitResults, true, DebugInfo.TraceColor,DebugInfo.TraceHitColor,DebugInfo.DrawTime);
	for (auto HitResult : HitResults)
	{
		ActorsToIgnore.Add(HitResult.GetActor());
		OnHitTarget.Broadcast(OwningActor.Get(), HitResult);
	}
	StartTraceLocation = EndTraceLocation;}

