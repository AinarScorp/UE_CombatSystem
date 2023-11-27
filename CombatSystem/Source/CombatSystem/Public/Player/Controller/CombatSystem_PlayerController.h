// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/CombatSystem_InputConfig.h"
#include "CombatSystem_PlayerController.generated.h"

class UCombatSystem_AbilityComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS(HideCategories = (Replication, HLOD, Cooking, DataLayers, WorldPatrition))
class COMBATSYSTEM_API ACombatSystem_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	FORCEINLINE FVector2D GetMoveInput() const {return MoveInput;};
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

private:
	void SetupInputs() const;
	void MoveInputTriggered(const FInputActionValue& InputActionValue);
	void MoveInputFinished(const FInputActionValue& InputActionValue);
	
	void LookInputTriggered(const FInputActionValue& InputActionValue);
	
	void AbilityInputTagPressedWithValue(const FInputActionValue& InputActionValue,FGameplayTag InputTag);
	void AbilityInputTagReleasedWithValue(const FInputActionValue& InputActionValue,FGameplayTag InputTag);
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	
	UCombatSystem_AbilityComponent* GetCombatASC();

public:
	friend class ACombatSystem_PlayerCharacter;
private:
	//Inputs
	UPROPERTY(EditDefaultsOnly, Category ="CombatSystem|PlayerController|Input", meta =(DisplayPriority = "1"))
	TObjectPtr<UCombatSystem_InputConfig> InputConfig;
	
	UPROPERTY(EditDefaultsOnly, Category ="CombatSystem|PlayerController|Input", meta =(DisplayPriority = "1"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY()
	TObjectPtr<UCombatSystem_AbilityComponent> CachedCombatAbilitySystemComponent;
	
	FVector2D MoveInput;

};
