// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "C_BTBoss.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UC_BTBoss : public UBTService
{
	GENERATED_BODY()

public:
	UC_BTBoss();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
