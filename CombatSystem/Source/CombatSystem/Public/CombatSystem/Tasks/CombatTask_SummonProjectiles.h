// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "CombatTask_SummonProjectiles.generated.h"

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileInfo")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileInfo")
	float ProjectileMoveSpeed = 100;
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileInfo")
	float ProjectileRadius = 10;
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileInfo")
	float OwningActorRadius = 10;

};
USTRUCT(BlueprintType)
struct FSpawnedProjectileInfo
{
	GENERATED_BODY()
	FSpawnedProjectileInfo(): Projectile(nullptr){};
	FSpawnedProjectileInfo(AActor* InProjectile)
	{
		Projectile = InProjectile;
		SpawnLocation = Projectile->GetActorLocation();

	}
	FSpawnedProjectileInfo(AActor* InProjectile, const FVector& InSpawnLocation)
	{
		Projectile = InProjectile;
		SpawnLocation = InSpawnLocation;
	}
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileInfo")
	AActor* Projectile;
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileInfo")
	FVector SpawnLocation;
	bool operator ==(const FSpawnedProjectileInfo& Other) const
	{
		return Projectile == Other.Projectile && SpawnLocation == Other.SpawnLocation;
	}
};
/**
 * 
 */
//TODO:Fix the name
UCLASS()
class COMBATSYSTEM_API UCombatTask_SummonProjectiles : public UGameplayTask
{
	GENERATED_BODY()
public:
	UCombatTask_SummonProjectiles(const FObjectInitializer& ObjectInitializer);

	static UCombatTask_SummonProjectiles* SummonProjectiles(AActor* OwningActor, const FProjectileInfo& ProjectileInfo,float NoiseCheckValue = 0,float MagicAttackRadius = 10,int MagicProjectilesCount = 20);
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectilesFinishedMoving);
	FOnProjectilesFinishedMoving OnTaskFinished;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileDestroyed, FVector, Location);
	FOnProjectileDestroyed OnProjectileDestroyed;
private:
	void DestroyProjectile(AActor*& Projectile);
	void MoveProjectilesInwards();
	bool TaskIsFinished() const;
private:
	TWeakObjectPtr<AActor> OwningActor;
	FProjectileInfo ProjectileInfo;
	float NoiseCheckValue;
	float MagicAttackRadius;
	int MagicProjectilesCount;
	UPROPERTY()
	TArray<FSpawnedProjectileInfo> SpawnedProjectilesInfos;
	float Alpha;
	FVector SummonPosition;
};
