// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "C_BTTaskTrace.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UC_BTTaskTrace : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UC_BTTaskTrace();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
