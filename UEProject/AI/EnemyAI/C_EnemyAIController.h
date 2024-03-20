// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_EnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AC_EnemyAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	class UC_BehaviorComponent* GetBehaviorComponent() { return BehaviorComponent; }

	class UAISenseConfig_Sight* GetSight() { return Sight; }

	float GetBehaviorRange() { return BehaviorRange; }

	void ChangeSight(float sightRadius, float loseSightRadius, float PVAD);

private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdateActors);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UAIPerceptionComponent* Perception;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UC_BehaviorComponent* BehaviorComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float BehaviorRange = 150.0f;

	class UAISenseConfig_Sight* Sight;

	class AC_Enemy* Owner;
};
