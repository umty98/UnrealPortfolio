// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WC_Name.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UWC_Name : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetName(const FText& name);
};
