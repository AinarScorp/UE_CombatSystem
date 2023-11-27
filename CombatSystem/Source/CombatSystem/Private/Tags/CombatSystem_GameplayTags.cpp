// Fill out your copyright notice in the Description page of Project Settings.


#include "Tags/CombatSystem_GameplayTags.h"

#include "GameplayTagsManager.h"
FCombatSystem_GameplayTags FCombatSystem_GameplayTags::GameplayTags;

void FCombatSystem_GameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

void FCombatSystem_GameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_Move, "InputTag.Move", "Move Input");
	AddTag(InputTag_Look, "InputTag.Look", "Look Input");
	AddTag(AttackInputWindow_Start, "AttackInputWindow.Start", "Used in AnimNotify to startListening for Input");
	AddTag(AttackInputWindow_End, "AttackInputWindow.End", "Used in AnimNotify to finish Listening for Input");
	AddTag(Weapon_StartExecute, "Weapon.StartExecute", "Used in AnimNotify to Start sphere tracing for a weapon");
	AddTag(Weapon_FinishExecute, "Weapon.FinishExecute", "Used in AnimNotify to finish sphere tracing for a weapon");
	
}

void FCombatSystem_GameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));

}
