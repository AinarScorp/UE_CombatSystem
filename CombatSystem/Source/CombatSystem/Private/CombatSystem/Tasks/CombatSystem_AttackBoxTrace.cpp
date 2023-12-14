// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_AttackBoxTrace.h"

#include "CombatSystem/Abilities/CombatAbility.h"
#include "Misc/MapErrors.h"


UCombatSystem_AttackBoxTrace* UCombatSystem_AttackBoxTrace::AttackBoxTrace(UCombatAbility* OwningAbility,USkeletalMeshComponent* SkeletalMeshComponent, const FName WeaponCenterSocketName, const FVector HalfSize, const FRotator RotationOffset, const ETraceTypeQuery TraceChannel, const TArray<AActor*> InActorsToIgnore)
{
	UCombatSystem_AttackBoxTrace* MyObj = NewTask<UCombatSystem_AttackBoxTrace>(OwningAbility, "AttackBoxTrace");
	MyObj->OwningAbility = OwningAbility;
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
	EndTraceLocation = SkeletalMesh->GetSocketLocation(WeaponCenterSocketName);
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMulti(SkeletalMesh.Get(),StartTraceLocation,EndTraceLocation,HalfSize,SkeletalMesh->GetComponentRotation() + RotationOffset,TraceChannel, false, ActorsToIgnore,DebugInfo.DrawDebugTraceType,HitResults, true, DebugInfo.TraceColor,DebugInfo.TraceHitColor,DebugInfo.DrawTime);
	for (auto HitResult : HitResults)
	{
		ActorsToIgnore.Add(HitResult.GetActor());
		OnHitTarget.Broadcast(OwningAbility->GetCurrentActorInfo()->AvatarActor.Get(), HitResult);
	}
	StartTraceLocation = EndTraceLocation;}

