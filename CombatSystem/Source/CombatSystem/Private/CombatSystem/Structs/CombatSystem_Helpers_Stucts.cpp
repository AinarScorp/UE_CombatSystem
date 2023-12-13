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
	// Need special case handling to maintain the explicit tag list correctly, adding the tag to the list if it didn't previously exist and a
	// positive delta comes in, and removing it from the list if it did exist and a negative delta comes in.
	
	if (!bTagAlreadyExplicitlyExists)
	{
		// Brand new tag with a positive delta needs to be explicitly added
		if (ToIncrease)
		{
			ExplicitTags.AddTag(Tag);
		}
		// Block attempted reduction of non-explicit tags, as they were never truly added to the container directly
		else
		{
			// only warn about tags that are in the container but will not be removed because they aren't explicitly in the container
			if (ExplicitTags.HasTag(Tag))
			{
				UE_LOG(LogTemp,Warning, TEXT("Attempted to remove tag: %s from tag count container, but it is not explicitly in the container!"), *Tag.ToString());
			}
			return false;
		}
	}

	// Update the explicit tag count map. This has to be separate than the map below because otherwise the count of nested tags ends up wrong
	int32& ExistingCount = ExplicitTagCountMap.FindOrAdd(Tag);

	const int32 CountDelta = ToIncrease? 1:-1;
	ExistingCount = FMath::Max(ExistingCount + CountDelta, 0);

	// If our new count is 0, remove us from the explicit tag list
	if (ExistingCount <= 0)
	{
		// Remove from the explicit list
		ExplicitTags.RemoveTag(Tag, true);
	}

	return true;
}

