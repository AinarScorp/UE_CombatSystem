// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/**
 * 
 */
struct FCombatSystem_GameplayTags
{
public:
	static const FCombatSystem_GameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();
protected:

	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
public:
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look;
	FGameplayTag InputTag_Attack;
	FGameplayTag InputTag_Attack_Primary;
	FGameplayTag InputTag_Attack_Secondary;
	FGameplayTag InputTag_Dodge;
	FGameplayTag InputTag_RightClick;
	FGameplayTag Actions_Attack_Combo;
	FGameplayTag Actions_Attack_Primary;
	FGameplayTag Actions_Attack_Secondary;
	FGameplayTag Actions_Attack_Sneak;
	FGameplayTag Actions_Attack_Magic;
	FGameplayTag Actions_Dodge;

	FGameplayTag Animation_NextAttackReady;
	FGameplayTag AttackInputWindow_Start;
	FGameplayTag AttackInputWindow_End;
	FGameplayTag Weapon;
	FGameplayTag Weapon_StartExecute;
	FGameplayTag Weapon_FinishExecute;
	FGameplayTag Hit;
	FGameplayTag Hit_Attack;
	FGameplayTag Hit_Sneak;

private:
	static FCombatSystem_GameplayTags GameplayTags;
};
