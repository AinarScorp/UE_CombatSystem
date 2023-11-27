// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/InputAbility.h"

UInputAbility::UInputAbility()
{
}

void UInputAbility::SetInputActionValue(const FInputActionValue& NewInputActionValue) const
{
	OnInputActionValuesSet.Broadcast(NewInputActionValue);
}

