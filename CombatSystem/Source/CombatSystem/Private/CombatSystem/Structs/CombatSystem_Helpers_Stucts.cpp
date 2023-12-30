// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Structs/CombatSystem_Helpers_Stucts.h"

void FGameplayTagContainerWithCount::RemoveTags(const FGameplayTagContainer& Container)
{
	bool bUpdatedAny = false;
	for (auto Tag = Container.CreateConstIterator(); Tag; ++Tag)
	{
		bUpdatedAny |= UpdateExplicitTags(*Tag,false);
	}
	if (bUpdatedAny)
	{
		ExplicitTags.FillParentTags();
	}
}

void FGameplayTagContainerWithCount::AddTags(const FGameplayTagContainer& Container)
{
	for (auto Tag = Container.CreateConstIterator(); Tag; ++Tag)
	{
		UpdateExplicitTags(*Tag,true);
	}
}

bool FGameplayTagContainerWithCount::UpdateExplicitTags(const FGameplayTag& Tag, const bool ToIncrease)
{
	const bool bTagAlreadyExplicitlyExists = ExplicitTags.HasTagExact(Tag);
	
	if (!bTagAlreadyExplicitlyExists)
	{
		if (ToIncrease)
		{
			ExplicitTags.AddTag(Tag);
		}
		else
		{
			if (ExplicitTags.HasTag(Tag))
			{
				UE_LOG(LogTemp,Warning, TEXT("Attempted to remove tag: %s from tag count container, but it is not explicitly in the container!"), *Tag.ToString());
			}
			return false;
		}
	}

	int32& ExistingCount = ExplicitTagCountMap.FindOrAdd(Tag);

	const int32 CountDelta = ToIncrease? 1:-1;
	ExistingCount = FMath::Max(ExistingCount + CountDelta, 0);

	if (ExistingCount <= 0)
	{
		ExplicitTags.RemoveTag(Tag, true);
	}

	return true;
}

