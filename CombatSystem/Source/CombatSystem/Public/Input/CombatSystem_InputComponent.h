// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem_InputConfig.h"
#include "EnhancedInputComponent.h"
#include "CombatSystem_InputComponent.generated.h"

/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatSystem_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename FuncType>
	void BindNativeAction(const UCombatSystem_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UCombatSystem_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActionsWithValue(const UCombatSystem_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);
};

template <class UserClass, typename FuncType>
void UCombatSystem_InputComponent::BindNativeAction(const UCombatSystem_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UCombatSystem_InputComponent::BindAbilityActions(const UCombatSystem_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FCombatInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (!Action.InputAction || !Action.InputTag.IsValid()) continue;

		if (PressedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle();
		}

		if (ReleasedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle();
		}
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UCombatSystem_InputComponent::BindAbilityActionsWithValue(const UCombatSystem_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FCombatInputAction& Action : InputConfig->AbilityInputActionsWithValues)
	{
		if (!Action.InputAction || !Action.InputTag.IsValid()) continue;

		if (PressedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle();
		}

		if (ReleasedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle();
		}
	}
}
