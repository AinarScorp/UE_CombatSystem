// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/CombatAbility_Combo.h"

#include "CombatSystem/Tasks/CombatSystem_PlayMontage.h"
#include "CombatSystem/Tasks/CombatSystem_WaitGameplayEvent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/Controller/CombatSystem_PlayerController.h"
#include "Tags/CombatSystem_GameplayTags.h"

void UCombatAbility_Combo::ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	Super::ActivateAbility(Handle, ActorInfo);
	CurrentComboIndex = 0;
	StartNextAttack();
}

void UCombatAbility_Combo::InputPressed(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo)
{
	if (!ListeningToInput) return;
	Super::InputPressed(Handle, ActorInfo);
	const FCombatSystem_GameplayTags& GameplayTags = FCombatSystem_GameplayTags::Get();
	
	UCombatSystem_WaitGameplayEvent* WaitEvent = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(ActorInfo->AvatarActor.Get(),GameplayTags.AttackInputWindow_End,true);
	WaitEvent->EventReceived.AddDynamic(this, &UCombatAbility_Combo::AttackInputWindowEnded);
	WaitEvent->ReadyForActivation();
	//CurrentActorInfo->CombatAbilitySystemComponent->AddTaskReadyForActivation(*WaitEvent);

	ListeningToInput = false;
}

void UCombatAbility_Combo::StartNextAttack()
{
	if (Combos.Num()<= CurrentComboIndex) return;

	const FComboAnimInfo& ComboInfo = Combos[CurrentComboIndex];
	CurrentComboIndex++;
	RotateToMoveInput();
	//UPlayMontageCallbackProxy* MontageTask = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(CurrentActorInfo->SkeletalMeshComponent.Get(),ComboInfo.AnimMontage,1,0,ComboInfo.AnimSection);
	UCombatSystem_PlayMontage* MontageTask = UCombatSystem_PlayMontage::CreatePlayMontageProxy(this, "AttackCombo", ComboInfo.AnimMontage, 1, ComboInfo.AnimSection);
	MontageTask->OnCompleted.AddDynamic(this, &UCombatAbility_Combo::MontageStoppedPlaying);
	//CurrentActorInfo->CombatAbilitySystemComponent->AddTaskReadyForActivation(*MontageTask);
	MontageTask->ReadyForActivation();
	const FCombatSystem_GameplayTags& GameplayTags = FCombatSystem_GameplayTags::Get();
	UCombatSystem_WaitGameplayEvent* WaitTask = UCombatSystem_WaitGameplayEvent::WaitGameplayEvent(CurrentActorInfo->AvatarActor.Get(),GameplayTags.AttackInputWindow_Start,true);
	WaitTask->EventReceived.AddDynamic(this, &UCombatAbility_Combo::AttackInputWindowStarted);
	WaitTask->ReadyForActivation();
	//CurrentActorInfo->CombatAbilitySystemComponent->AddTaskReadyForActivation(*WaitTask);
}

void UCombatAbility_Combo::MontageStoppedPlaying()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, false);
}

void UCombatAbility_Combo::AttackInputWindowStarted(FCombatEventData Payload)
{
	ListeningToInput = true;
}

void UCombatAbility_Combo::AttackInputWindowEnded(FCombatEventData Payload)
{
	StartNextAttack();
}

void UCombatAbility_Combo::RotateToMoveInput() const
{
	if (!bShouldRotateToInput || !CurrentActorInfo->Controller.Get()) return;
	const ACombatSystem_PlayerController* PlayerController = Cast<ACombatSystem_PlayerController>(CurrentActorInfo->Controller);
	if (!PlayerController) return;

	const FVector2D MoveInput =PlayerController->GetMoveInput();
	if (MoveInput == FVector2D::ZeroVector) return;
	const FRotator RotFromMoveInput = UKismetMathLibrary::MakeRotFromX(FVector(MoveInput.X, MoveInput.Y,0));
	const FRotator RotFromCurrentRotation = FRotator(0, CurrentActorInfo->Controller->GetControlRotation().Yaw,0);
	
	CurrentActorInfo->AvatarActor->SetActorRotation(UKismetMathLibrary::ComposeRotators(RotFromMoveInput, RotFromCurrentRotation));
}


