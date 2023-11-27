// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Helpers/GameplayLibraryHelper.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EinarGameplayLibrary.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UEinarGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static ERelativeContext GetActorRelativeContext(const AActor* Target, const FVector& Location);
};
