// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AllyAI/C_BTDeathAlly.h"
#include "AI/AllyAI/C_AllyAIController.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"
#include "GameFramework/CharacterMovementComponent.h"

UC_BTDeathAlly::UC_BTDeathAlly()
{
	NodeName = "DeathAlly";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTDeathAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_AllyAIController *controller = Cast<AC_AllyAIController>(OwnerComp.GetOwner());

	AC_AllyWarrior* ally = Cast<AC_AllyWarrior>(controller->GetPawn());

	if (ally)
	{
		UCharacterMovementComponent* CharMovement = ally->GetCharacterMovement();
		if (CharMovement)
		{
			CharMovement->MaxWalkSpeed = 0.0f;
		}

		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}
