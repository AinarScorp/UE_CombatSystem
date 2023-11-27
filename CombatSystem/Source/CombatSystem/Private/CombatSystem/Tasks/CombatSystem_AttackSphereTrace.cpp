// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_AttackSphereTrace.h"

#include "Kismet/KismetSystemLibrary.h"




UCombatSystem_AttackSphereTrace* UCombatSystem_AttackSphereTrace::AttackSphereTrace(AActor* OwningActor, USkeletalMeshComponent* SkeletalMeshComponent, const FName WeaponCenterSocketName, const float Radius, const ETraceTypeQuery TraceChannel, const TArray<AActor*> InActorsToIgnore)
{
	UCombatSystem_AttackSphereTrace* MyObj = NewTask<UCombatSystem_AttackSphereTrace>(OwningActor, "AttackSphereTrace");
	MyObj->OwningActor = OwningActor;
	MyObj->SkeletalMesh = SkeletalMeshComponent;
	MyObj->WeaponCenterSocketName = WeaponCenterSocketName;
	MyObj->Radius = Radius;
	MyObj->TraceChannel = TraceChannel;
	MyObj->ActorsToIgnore = InActorsToIgnore;
	return MyObj;
}

void UCombatSystem_AttackSphereTrace::TickTask(float DeltaTime)
{
	EndTraceLocation = SkeletalMesh.Get()->GetSocketLocation(WeaponCenterSocketName);
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(OwningActor.Get(),StartTraceLocation,EndTraceLocation,Radius,TraceChannel, false, ActorsToIgnore,DebugInfo.DrawDebugTraceType,HitResults, true, DebugInfo.TraceColor,DebugInfo.TraceHitColor,DebugInfo.DrawTime);
	for (auto HitResult : HitResults)
	{
		ActorsToIgnore.Add(HitResult.GetActor());
		OnHitTarget.Broadcast(OwningActor.Get(), HitResult);
	}
	StartTraceLocation = EndTraceLocation;
}
