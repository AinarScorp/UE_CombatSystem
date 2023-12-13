// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Abilities/MagicAttack.h"

void UMagicAttack::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, TriggerEventData);
	FVector AvatarLocation = ActorInfo->AvatarActor->GetActorLocation();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(ActorInfo->AvatarActor.Get());
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(ActorInfo->AvatarActor.Get(),AvatarLocation, AvatarLocation,MagicAttackRadius, EnemyChannel, false,ActorsToIgnore,TraceDebugInfo.DrawDebugTraceType,HitResults,true,TraceDebugInfo.TraceColor,TraceDebugInfo.TraceHitColor, TraceDebugInfo.DrawTime);
	for (auto HitResult : HitResults)
	{
		FVector HitActorLocation = HitResult.GetActor()->GetActorLocation();
		FVector2D Location{AvatarLocation.X + HitActorLocation.X, AvatarLocation.Y + HitActorLocation.Y};
		DrawDebugSphere(ActorInfo->AvatarActor.Get()->GetWorld(),HitActorLocation,25,12,FColor::Emerald,false,2,0,2);
		UE_LOG(LogTemp,Warning, TEXT("NameOf Enemy: %s with Noise %f"), *HitResult.GetActor()->GetName(),FMath::PerlinNoise2D(Location))
	}
	InternalEndAbility();
}
