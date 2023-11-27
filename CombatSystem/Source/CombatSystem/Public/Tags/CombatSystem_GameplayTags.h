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
	FGameplayTag AttackInputWindow_Start;
	FGameplayTag AttackInputWindow_End;
	FGameplayTag Weapon;
	FGameplayTag Weapon_StartExecute;
	FGameplayTag Weapon_FinishExecute;
	FGameplayTag Hit;

private:
	static FCombatSystem_GameplayTags GameplayTags;
};
