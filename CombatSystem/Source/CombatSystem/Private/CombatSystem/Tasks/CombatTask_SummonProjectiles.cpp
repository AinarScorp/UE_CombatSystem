// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatTask_SummonProjectiles.h"

#include "CombatSystem/Abilities/SurroundAttack.h"
#include "Library/EinarMathLibrary.h"



UCombatTask_SummonProjectiles::UCombatTask_SummonProjectiles(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bTickingTask =true;
	Alpha = 0.f;
}

UCombatTask_SummonProjectiles* UCombatTask_SummonProjectiles::SummonProjectiles(AActor* OwningActor, const FProjectileInfo& ProjectileInfo,float NoiseCheckValue, const float MagicAttackRadius, const int MagicProjectilesCount)
{
	UCombatTask_SummonProjectiles* MyObj = NewTask<UCombatTask_SummonProjectiles>(OwningActor, "Projectiles Attack");
	MyObj->OwningActor=  OwningActor;
	MyObj->ProjectileInfo=  ProjectileInfo;
	MyObj->NoiseCheckValue=  FMath::Clamp(NoiseCheckValue, -1,1);
	MyObj->MagicAttackRadius = MagicAttackRadius;
	MyObj->MagicProjectilesCount = MagicProjectilesCount;
	return MyObj;
	
}

void UCombatTask_SummonProjectiles::Activate()
{
	Super::Activate();
	if (!OwningActor.IsValid() || !ProjectileInfo.ProjectileClass)
	{
		EndTask();
	}
	SummonPosition = OwningActor->GetActorLocation();
	const FVector2D AvatarLocation{OwningActor->GetActorLocation()};
	for (int i = 0; i <= MagicProjectilesCount; ++i)
	{
		float Angle = (float)i/(float)MagicProjectilesCount * 355.f;
		FVector2D DirectionVector = {FMath::Sin(FMath::RadiansToDegrees(Angle)), FMath::Cos(FMath::RadiansToDegrees(Angle))};

		const float PerlinNoise = FMath::PerlinNoise2D(AvatarLocation + DirectionVector* MagicAttackRadius);
		FVector SpawnLocation = SummonPosition +FVector(DirectionVector.X,DirectionVector.Y,0) * MagicAttackRadius;

		if (PerlinNoise>=0)
		{
			SpawnedProjectilesInfos.Add(FSpawnedProjectileInfo(OwningActor.Get()->GetWorld()->SpawnActor<AActor>(ProjectileInfo.ProjectileClass,SpawnLocation, FRotator())));
		}
	}
	
}

void UCombatTask_SummonProjectiles::TickTask(float DeltaTime)
{
	MoveProjectilesInwards();
	
	Alpha+=DeltaTime * ProjectileInfo.ProjectileMoveSpeed;
	UE_LOG(LogTemp, Display, TEXT("%f : ALpha"), Alpha);
	if (TaskIsFinished())
	{
		OnTaskFinished.Broadcast();
		EndTask();
	}
}

void UCombatTask_SummonProjectiles::OnDestroy(bool bInOwnerFinished)
{
	for (auto& SpawnedProjectileInfo : SpawnedProjectilesInfos)
	{
		DestroyProjectile(SpawnedProjectileInfo.Projectile);
	}
	Super::OnDestroy(bInOwnerFinished);
	
}

void UCombatTask_SummonProjectiles::DestroyProjectile(AActor*& Projectile)
{
	OnProjectileDestroyed.Broadcast(Projectile->GetActorLocation());
	Projectile->Destroy();
}

void UCombatTask_SummonProjectiles::MoveProjectilesInwards()
{
	TArray<FSpawnedProjectileInfo> SpawnedProjectilesToRemove;
	for (const auto& SpawnedProjectileInfo : SpawnedProjectilesInfos)
	{
		AActor* Projectile = SpawnedProjectileInfo.Projectile;
		Projectile->SetActorLocation(FMath::Lerp(SpawnedProjectileInfo.SpawnLocation,SummonPosition,Alpha));
		if(UEinarMathLibrary::SphereSphereIntersection(Projectile->GetActorLocation(),ProjectileInfo.ProjectileRadius, OwningActor->GetActorLocation(),ProjectileInfo.OwningActorRadius)||
			FMath::PerlinNoise2D(FVector2D(Projectile->GetActorLocation().X, Projectile->GetActorLocation().Y))>NoiseCheckValue)
		{
			SpawnedProjectilesToRemove.Add(SpawnedProjectileInfo);
		}
		
	}

	for (auto& ProjectileToRemove : SpawnedProjectilesToRemove)
	{
		AActor* Projectile = ProjectileToRemove.Projectile;
		SpawnedProjectilesInfos.Remove(ProjectileToRemove);
		DestroyProjectile(Projectile);
	}
}

bool UCombatTask_SummonProjectiles::TaskIsFinished() const
{
	return Alpha>=1.f;
}

