// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WC_PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UWC_PlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	//Warrior
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHp(const float& CurHp, const float& MaxHp);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMp(const float& CurMp, const float& MaxMp);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateExp(const float& CurExp, const float& MaxExp, const int32& Level);

	//Boss
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBossHp(const float& CurHp, const float& MaxHp);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBossHpBox(const bool active);

	//weaponicon
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFistIcon(const bool active);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSwordIcon(const bool active);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateBowIcon(const bool active);

	//skillcooltime-common
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDashCoolTime(const bool cooltime);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateKickCoolTime(const bool cooltime);

	//
	//skillcooltime-sword-Q
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateQSIcon(const bool active);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateQSCoolTime(int32 TimeRemaining);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateQSTimerActive(const bool timeractive);
	//skillcooltime-sword-E
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateESIcon(const bool active);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateESCoolTime(int32 TimeRemaining);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateESTimerActive(const bool timeractive);
	//skillcooltime-bow-Q
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateQBIcon(const bool active);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateQBCoolTime(int32 TimeRemaining);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateQBTimerActive(const bool timeractive);
	//skillcooltime-bow-Q
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEBIcon(const bool active);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEBCoolTime(int32 TimeRemaining);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEBTimerActive(const bool timeractive);

	//Map
	UFUNCTION(BlueprintImplementableEvent)
	void WorldMapOn(const bool bWorldMap);

	UFUNCTION(BlueprintImplementableEvent)
	void MiniMapOn(const bool bMiniMap);

	//Stun&Freezed
	UFUNCTION(BlueprintImplementableEvent)
	void IsStunned(const bool bStun);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStunBar(float stuntime, float fullstuntime);

	UFUNCTION(BlueprintImplementableEvent)
	void IsFreezed(const bool bFreezed);

};
