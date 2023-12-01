// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/CombatAbility.h"
#include "InputAbility.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputActionValueSet,const FInputActionValue&, InputActionValue);
UCLASS(Abstract, Blueprintable, BlueprintType)
class COMBATSYSTEM_API UInputAbility : public UCombatAbility
{
	GENERATED_BODY()
	
public:
	void SetInputActionValue(const FInputActionValue& NewInputActionValue) const;

protected:
	UPROPERTY(BlueprintAssignable)
	FOnInputActionValueSet OnInputActionValuesSet;
};
