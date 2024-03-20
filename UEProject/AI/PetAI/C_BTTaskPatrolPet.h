// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "C_BTTaskPatrolPet.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UC_BTTaskPatrolPet : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UC_BTTaskPatrolPet();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);


};
