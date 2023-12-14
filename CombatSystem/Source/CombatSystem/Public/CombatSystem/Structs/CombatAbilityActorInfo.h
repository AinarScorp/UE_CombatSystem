// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbilityActorInfo.generated.h"

class UCombatSystemComponent;

USTRUCT(BlueprintType)
struct FCombatAbilityActorInfo
{
	GENERATED_BODY()
	/** The actor that owns the abilities, shouldn't be null */
	UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	TWeakObjectPtr<AActor> OwnerActor;

	UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	TWeakObjectPtr<AActor> AvatarActor;
	UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	TWeakObjectPtr<UAnimInstance> AnimInstance;
	UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	TWeakObjectPtr<UCombatSystemComponent>	CombatSystemComponent;
	//TODO: Maybe add Player controller and AI controller to avoid casting?
	UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	TWeakObjectPtr<AController>	Controller;

	/** Initializes the info from an owning actor. Will set both owner and avatar */
	virtual void InitFromActor(AActor *OwnerActor, AActor *AvatarActor, UCombatSystemComponent* InAbilitySystemComponent);
	void Test(USkeletalMeshComponent* InSkeletalMeshComponent)
	{
		SkeletalMeshComponent = InSkeletalMeshComponent;
	};
};
