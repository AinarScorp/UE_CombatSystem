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
	static ERelativeContext GetActorRelativeContextSingle(const AActor* ThisActor, const FVector& Location);
	UFUNCTION(BlueprintCallable, DisplayName = "Get Actor Relative Context")
	static ERelativeContext_BP GetActorRelativeContext_BP(const AActor* Target, const FVector& Location);
	UFUNCTION(BlueprintCallable, Category = "Context", meta = ( DefaultToSelf = Target ))
	static int32 GetRelativeContext(const AActor* ThisActor, const AActor* Other);
	// Used for checking a group of flags
	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool ContextPredicate(const int32 Test, const int32 Value);

};
