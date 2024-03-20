// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_BTTaskDeath.h"
#include "AI/EnemyAI/C_EnemyAIController.h"
#include "Unit/Enemy/C_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UC_BTTaskDeath::UC_BTTaskDeath()
{
	NodeName = "Death";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskDeath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());

	AC_Enemy* enemy = Cast<AC_Enemy>(controller->GetPawn());

	if (enemy)
	{
		UCharacterMovementComponent* CharMovement = enemy->GetCharacterMovement();
		if (CharMovement)
		{
			CharMovement->MaxWalkSpeed = 0.0f;
		}

		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}

void UC_BTTaskDeath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
