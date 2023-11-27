// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatSystem_AbilityInterface.h"
#include "CombatSystem_CharacterBase.generated.h"

class UCombatSystem_CharacterStartupInfo;
class UCombatAbility;


UCLASS()
class COMBATSYSTEM_API ACombatSystem_CharacterBase : public ACharacter, public ICombatSystem_AbilityInterface
{
	GENERATED_BODY()

public:
	ACombatSystem_CharacterBase();

	virtual UCombatSystem_AbilityComponent* GetCombatAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;


private:
	
	UPROPERTY(EditAnywhere, Category = "CharacterBase|CombatSystem")
	TObjectPtr<UCombatSystem_CharacterStartupInfo> StartupInfo;
	UPROPERTY(VisibleAnywhere, Category = "CharacterBase|CombatSystem")
	TObjectPtr<UCombatSystem_AbilityComponent> CombatSystemComponent;
};
