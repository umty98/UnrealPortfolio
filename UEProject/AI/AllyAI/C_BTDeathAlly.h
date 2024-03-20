// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "C_BTDeathAlly.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UC_BTDeathAlly : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UC_BTDeathAlly();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
