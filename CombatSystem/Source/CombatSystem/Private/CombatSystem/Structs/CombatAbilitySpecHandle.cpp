// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Structs/CombatAbilitySpecHandle.h"


void FCombatAbilitySpecHandle::GenerateNewHandle()
{
	// Must be in C++ to avoid duplicate statics accross execution units
	static int32 GHandle = 1;
	Handle = GHandle++;
}
