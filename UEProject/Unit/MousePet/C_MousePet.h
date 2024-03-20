// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit/C_Unit.h"
#include "C_MousePet.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_MousePet : public AC_Unit
{
	GENERATED_BODY()

public:
	AC_MousePet();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	class UBehaviorTree* GetBT() { return BehaviorTree; }

	class UC_PetPatrolComponent* GetPatrolComponent() { return PatrolComponent; }

	void Landed(const FHitResult& Hit);

	bool& GetIsFloating() { return bIsFloating; }

	void PetFloating();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UC_PetPatrolComponent* PatrolComponent;

};
