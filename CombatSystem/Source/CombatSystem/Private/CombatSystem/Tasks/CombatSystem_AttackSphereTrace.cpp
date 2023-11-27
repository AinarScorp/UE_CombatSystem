// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_AttackSphereTrace.h"

#include "Kismet/KismetSystemLibrary.h"




UCombatSystem_AttackSphereTrace::UCombatSystem_AttackSphereTrace(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask =true;

}

UCombatSystem_AttackSphereTrace* UCombatSystem_AttackSphereTrace::AttackSphereTrace(AActor* OwningActor)
{
	return nullptr;
}

void UCombatSystem_AttackSphereTrace::TickTask(float DeltaTime)
{
	// USkeletalMeshComponent* SkeletalMeshComponent;
	// FVector Start = SkeletalMeshComponent->GetSocketLocation("StartSocket");
	// FVector End = SkeletalMeshComponent->GetSocketLocation("EndSocket");
	// float Radius = 2;
	// ETraceTypeQuery TypeQuery;
	// UKismetSystemLibrary::SphereTraceMulti(nullptr,Start,End,Radius,TypeQuery, false, )
}

void UCombatSystem_AttackSphereTrace::Activate()
{
	Super::Activate();
}
