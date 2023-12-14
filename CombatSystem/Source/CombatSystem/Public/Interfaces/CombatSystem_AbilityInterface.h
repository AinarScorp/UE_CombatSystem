// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatSystem_AbilityInterface.generated.h"

class UCombatSystemComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatSystem_AbilityInterface : public UInterface
{
	GENERATED_BODY()
};


class COMBATSYSTEM_API ICombatSystem_AbilityInterface
{
	GENERATED_BODY()

public:
	virtual UCombatSystemComponent* GetCombatSystemComponent()const =0;
};
