// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/C_Weapon.h"
#include "C_Orb.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_Orb : public AC_Weapon
{
	GENERATED_BODY()

public:
	AC_Orb();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void ActiveShield();
	void ActiveMiddleAttack();
	void ActiveFallingSpear();
	void ActiveFollowingSpear();
};
