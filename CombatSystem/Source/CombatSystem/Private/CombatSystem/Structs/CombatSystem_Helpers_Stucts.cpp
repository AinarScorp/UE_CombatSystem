// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Structs/CombatSystem_Helpers_Stucts.h"

void FGameplayTagContainerWithCount::RemoveTags(FGameplayTagContainer& Container)
{
	//TODO: ask Martin about this for loop
	for (auto Tag = Container.CreateConstIterator(); Tag; ++Tag)
	{
		
	}
}
