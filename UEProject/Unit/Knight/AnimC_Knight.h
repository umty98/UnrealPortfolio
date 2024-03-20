// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Unit/AnimC_Unit.h"
#include "C_Knight.h"
#include "Component/C_FeetComponent.h"
#include "AnimC_Knight.generated.h"
/**
 * 
 */
UCLASS()
class UEPROJECT_API UAnimC_Knight : public UAnimC_Unit
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bFlip;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FFeetData FeetData;
};
