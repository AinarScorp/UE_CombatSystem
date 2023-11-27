// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CombatSystem_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_AssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UCombatSystem_AssetManager& Get();
protected:
	virtual void StartInitialLoading() override;
};
