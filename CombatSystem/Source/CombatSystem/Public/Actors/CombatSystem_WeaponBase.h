// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatSystem_WeaponBase.generated.h"

class USkeletalMeshComponent_Weapon;

UCLASS(BlueprintType,Blueprintable,Abstract)
class COMBATSYSTEM_API ACombatSystem_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACombatSystem_WeaponBase();
	USkeletalMeshComponent* GetWeaponMesh() const;
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "WeaponBase", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent_Weapon* WeaponMesh;
};
