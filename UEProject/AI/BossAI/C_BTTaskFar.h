// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "C_BTTaskFar.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UC_BTTaskFar : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UC_BTTaskFar();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
