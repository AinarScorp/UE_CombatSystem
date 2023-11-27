// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CombatSystem_WeaponBase.h"

#include "Components/Actor/SkeletalMeshComponent_Weapon.h"

// Sets default values
ACombatSystem_WeaponBase::ACombatSystem_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent_Weapon>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}


