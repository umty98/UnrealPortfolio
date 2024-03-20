// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit/C_Unit.h"
#include "C_AllyWarrior.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_AllyWarrior : public AC_Unit
{
	GENERATED_BODY()

public:
	AC_AllyWarrior();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	class UBehaviorTree* GetBT() { return BehaviorTree; }

	bool GetCanAttack() { return bCanAttack; }
	bool& SetCanAttack() { return bCanAttack; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

private:
	bool bCanAttack = false;
};
