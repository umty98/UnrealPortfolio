// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "C_Hud.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_Hud : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	UPROPERTY()
	class UWC_PlayerUI* PlayerUIInstance;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UTexture2D* TargetedTexture;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UTexture2D* NontargetedCrosshair;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWC_PlayerUI> PlayerUIClass;

	
};
