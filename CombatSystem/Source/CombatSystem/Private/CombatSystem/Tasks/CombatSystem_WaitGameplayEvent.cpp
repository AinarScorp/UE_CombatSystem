// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"

#include "GameplayTagContainer.h"
#include "Components/Actor/CombatSystem_AbilityComponent.h"
#include "Interfaces/CombatSystem_AbilityInterface.h"

UCombatSystem_WaitGameplayEvent* UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(AActor* TargetActor, FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UCombatSystem_WaitGameplayEvent* MyObj = NewTask<UCombatSystem_WaitGameplayEvent>(TargetActor, "WaitGameplayEvent");

	const ICombatSystem_AbilityInterface* CombatAbilityInterface = Cast<ICombatSystem_AbilityInterface>(TargetActor);
	if (CombatAbilityInterface)
	{
		MyObj->CombatSystemComponent = CombatAbilityInterface->GetCombatAbilitySystemComponent();
	}
	MyObj->Tag = EventTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	MyObj->OnlyMatchExact = OnlyMatchExact;
	return MyObj;
}

void UCombatSystem_WaitGameplayEvent::Activate()
{
	UCombatSystem_AbilityComponent* CSC = CombatSystemComponent.Get();
	if (CSC)
	{
		if (OnlyMatchExact)
		{
			MyHandle = CSC->GenericGameplayEventCallbacks.FindOrAdd(Tag).AddUObject(this, &UCombatSystem_WaitGameplayEvent::GameplayEventCallback);
		}
		else
		{
			//MyHandle = CSC->AddGameplayEventTagContainerDelegate(FGameplayTagContainer(Tag), FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAbilityAsync_WaitGameplayEvent::GameplayEventContainerCallback));
		}
	}
	Super::Activate();
}

void UCombatSystem_WaitGameplayEvent::EndAction()
{
	UCombatSystem_AbilityComponent* CSC = CombatSystemComponent.Get();
	if (CSC && MyHandle.IsValid())
	{
		if (OnlyMatchExact)
		{
			CSC->GenericGameplayEventCallbacks.FindOrAdd(Tag).Remove(MyHandle);
		}
		else
		{
			//CSC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(Tag), MyHandle);
		}

	}
}

void UCombatSystem_WaitGameplayEvent::OnDestroy(bool bInOwnerFinished)
{
	EndAction();
	Super::OnDestroy(bInOwnerFinished);
}


void UCombatSystem_WaitGameplayEvent::GameplayEventCallback(const FCombatEventData* Payload)
{
	UE_LOG(LogTemp,Display, TEXT("%s GameplayEventCallback %s"),*GetName(), *Tag.GetTagName().ToString())

	GameplayEventContainerCallback(Tag, Payload);

}

void UCombatSystem_WaitGameplayEvent::GameplayEventContainerCallback(FGameplayTag MatchingTag, const FCombatEventData* Payload)
{
	if (CombatSystemComponent.Get())
	{
		FCombatEventData TempPayload = *Payload;
		TempPayload.EventTag = MatchingTag;
		EventReceived.Broadcast(MoveTemp(TempPayload));
		
	}
	if (OnlyTriggerOnce)
	{
		EndTask();
	}

}
