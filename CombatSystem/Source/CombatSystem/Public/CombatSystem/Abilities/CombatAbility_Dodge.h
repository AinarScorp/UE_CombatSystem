// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "Helpers/GameplayLibraryHelper.h"
#include "Helpers/HelperStructs.h"
#include "CombatAbility_Dodge.generated.h"


class UCombatSystem_PlayMontage;

USTRUCT(BlueprintType)
struct FDodgeMontage
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FMontageWithSection Montage;
	UPROPERTY(EditDefaultsOnly, meta=(Bitmask, BitmaskEnum = "/Script/CombatSystem.ERelativeContext"))
	int32 Direction;
};
/**
 * 
 */
UCLASS()
class COMBATSYSTEM_API UCombatAbility_Dodge : public UCombatAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, const FCombatEventData* TriggerEventData) override;
	virtual void EndAbility(const FCombatAbilitySpecHandle Handle, const FCombatAbilityActorInfo* ActorInfo, bool bWasCancelled) override;
private:
	FMontageWithSection* GetDodgeMontage();
	FMontageWithSection* GetDodgeMontageFromLocking();
	FMontageWithSection* GetDodgeMontageFromContext(ERelativeContext RelativeContext);
	bool RotateToMoveInput() const;
	void AssignDodgeDirection(const FVector& Direction);
	void AssignDodgeDirection(ERelativeContext RelativeContext);
private:
	UPROPERTY(EditDefaultsOnly, Category="Dodge|Settings")
	TArray<FDodgeMontage> DodgeMontages;

	TWeakObjectPtr<UCombatSystem_PlayMontage> MontageTask;
	UPROPERTY(EditDefaultsOnly, Category="Dodge|Settings")
	bool UseRootMotion = false;
	UPROPERTY(EditDefaultsOnly, Category="Dodge|Settings", meta = (EditConditionHides,EditCondition = "UseRootMotion == false"))
	float DodgeMoveSpeed = 100;
	FVector DodgeDirection;
	
};
