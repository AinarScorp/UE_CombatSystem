// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/CombatSystem_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CombatSystem_PlayerCharacter.h"
#include "Components/Actor/CombatSystem_AbilityComponent.h"
#include "Input/CombatSystem_InputComponent.h"
#include "Tags/CombatSystem_GameplayTags.h"

void ACombatSystem_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetupInputs();
}


void ACombatSystem_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputConfig);
	UCombatSystem_InputComponent* CombatSystemInputComponent = CastChecked<UCombatSystem_InputComponent>(InputComponent);
	const FCombatSystem_GameplayTags& GameplayTags = FCombatSystem_GameplayTags::Get();

	//Move action bindings
	//CombatSystemInputComponent->BindAction()
	CombatSystemInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ACombatSystem_PlayerController::MoveInputTriggered);
	CombatSystemInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Canceled, this, &ACombatSystem_PlayerController::MoveInputFinished);
	CombatSystemInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Completed, this, &ACombatSystem_PlayerController::MoveInputFinished);

	CombatSystemInputComponent->BindNativeAction(InputConfig, GameplayTags.InputTag_Look, ETriggerEvent::Triggered, this, &ACombatSystem_PlayerController::LookInputTriggered);

	CombatSystemInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased);
	CombatSystemInputComponent->BindAbilityActionsWithValue(InputConfig, this, &ThisClass::AbilityInputTagPressedWithValue, &ThisClass::AbilityInputTagReleasedWithValue);
}

void ACombatSystem_PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (GetCombatASC())
	{
		GetCombatASC()->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}


void ACombatSystem_PlayerController::SetupInputs() const
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem && DefaultMappingContext)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}


void ACombatSystem_PlayerController::MoveInputTriggered(const FInputActionValue& InputActionValue)
{
	MoveInput = InputActionValue.Get<FVector2D>();
}

void ACombatSystem_PlayerController::MoveInputFinished(const FInputActionValue& InputActionValue)
{
	MoveInput = {0, 0};
}

void ACombatSystem_PlayerController::LookInputTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}


void ACombatSystem_PlayerController::AbilityInputTagPressedWithValue(const FInputActionValue& InputActionValue, FGameplayTag InputTag)
{
	GetCombatASC()->AbilityInputTagPressedWithValue(InputActionValue, InputTag);
}

void ACombatSystem_PlayerController::AbilityInputTagReleasedWithValue(const FInputActionValue& InputActionValue, FGameplayTag InputTag)
{
	GetCombatASC()->AbilityInputTagReleasedWithValue(InputActionValue, InputTag);
}

void ACombatSystem_PlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GetCombatASC()->AbilityInputTagPressed(InputTag);
}

void ACombatSystem_PlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GetCombatASC()->AbilityInputTagReleased(InputTag);
}

UCombatSystem_AbilityComponent* ACombatSystem_PlayerController::GetCombatASC()
{
	if (CachedCombatAbilitySystemComponent == nullptr && GetPawn<ACombatSystem_CharacterBase>())
	{
		CachedCombatAbilitySystemComponent = GetPawn<ACombatSystem_CharacterBase>()->GetCombatAbilitySystemComponent();
	}
	return CachedCombatAbilitySystemComponent;
}

// void ACombatSystem_PlayerController::AttackPressInputTriggered(const FInputActionValue& InputActionValue)
// {
// 	AttackInput = true;
// }
//
