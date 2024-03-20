// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "C_BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_BossAIController : public AAIController
{
	GENERATED_BODY()
public:
	AC_BossAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	class UC_BehaviorComponent* GetBehaviorComponent() { return BehaviorComponent; }

	class UAISenseConfig_Sight* GetSight() { return Sight; }

	float GetBehaviorRangeNear() { return BehaviorRangeNear; }
	float GetBehaviorRangeMiddle() { return BehaviorRangeMiddle; }
	float GetBehaviorRangeFar() { return BehaviorRangeFar; }


private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdateActors);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UAIPerceptionComponent* Perception;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UC_BehaviorComponent* BehaviorComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float BehaviorRangeNear = 500.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float BehaviorRangeMiddle = 1500.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float BehaviorRangeFar = 3000.0f;


	class UAISenseConfig_Sight* Sight;
	
	class AC_Boss* Owner;
};
