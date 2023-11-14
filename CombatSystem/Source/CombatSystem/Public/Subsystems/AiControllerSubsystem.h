// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AiControllerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UAiControllerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<AActor*> ActiveEnemies;
public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<AActor*> GetActiveEnemies() { return ActiveEnemies;}
	UFUNCTION(BlueprintCallable)
	void AddToActiveEnemies(AActor* NewActiveEnemy) {ActiveEnemies.AddUnique(NewActiveEnemy);}
	UFUNCTION(BlueprintCallable)
	void RemoveActiveEnemies(AActor* ActiveEnemyToRemove) {ActiveEnemies.RemoveSingle(ActiveEnemyToRemove);}

};
