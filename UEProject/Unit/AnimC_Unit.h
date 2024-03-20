// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C_Unit.h"

#include "AnimC_Unit.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UAnimC_Unit : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class AC_Unit* Owner;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EWeaponState WeaponType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bFalling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bStunned;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bCanStandUp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bCanBlock;
};
