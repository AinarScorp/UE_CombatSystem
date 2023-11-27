// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CombatSystem_AssetManager.h"

#include "Tags/CombatSystem_GameplayTags.h"

UCombatSystem_AssetManager& UCombatSystem_AssetManager::Get()
{
	check(GEngine);

	UCombatSystem_AssetManager* AuraAssetManager = Cast<UCombatSystem_AssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UCombatSystem_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FCombatSystem_GameplayTags::InitializeNativeTags();

}
