// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "SkeletalMeshComponent_Weapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,HideCategories = (Replication, AssetUserData, RayTracing, Mobile, TextureStreming, Optimization, Cooking, ComponentReplication, HLOD, SkinWeights,Lighting,Deformer))
class COMBATSYSTEM_API USkeletalMeshComponent_Weapon : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void BeginTrail(UParticleSystemComponent* TrailParticleComponent, ETrailWidthMode InWidthMode = ETrailWidthMode_FromCentre, float InWidth = 1);
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetTrail(UParticleSystemComponent* TrailParticleComponent, ETrailWidthMode InWidthMode = ETrailWidthMode_FromCentre, float InWidth = 1);
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void EndTrail(UParticleSystemComponent* TrailParticleComponent);
};
