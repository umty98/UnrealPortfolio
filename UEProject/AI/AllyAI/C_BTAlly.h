// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "C_BTAlly.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UC_BTAlly : public UBTService
{
	GENERATED_BODY()

public:
	UC_BTAlly();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
