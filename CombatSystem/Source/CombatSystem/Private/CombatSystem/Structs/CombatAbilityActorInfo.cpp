// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Structs/CombatAbilityActorInfo.h"

void FCombatAbilityActorInfo::InitFromActor(AActor* InOwnerActor, AActor* InAvatarActor, UCombatSystem_AbilityComponent* InAbilitySystemComponent)
{
	OwnerActor = InOwnerActor;
	AvatarActor = InAvatarActor;
	CombatAbilitySystemComponent = InAbilitySystemComponent;
	if (const AActor* AvatarActorPtr = AvatarActor.Get())
	{
		SkeletalMeshComponent = AvatarActorPtr->FindComponentByClass<USkeletalMeshComponent>();
		
	}
	if (SkeletalMeshComponent.Get())
	{
		AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	}
	if (const APawn* Pawn = Cast<APawn>(InAvatarActor); Pawn && Pawn->GetController())
	{
		Controller = Pawn->GetController();
	}
}