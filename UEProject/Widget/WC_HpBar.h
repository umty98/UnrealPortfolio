// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WC_HpBar.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UWC_HpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHp(const float& CurHp, const float& MaxHp);
};
