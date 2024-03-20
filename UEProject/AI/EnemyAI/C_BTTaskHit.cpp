// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_BTTaskHit.h"
#include "AI/EnemyAI/C_EnemyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/Enemy/C_Enemy.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"

UC_BTTaskHit::UC_BTTaskHit()
{
	NodeName = "Hit";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskHit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	if (!controller)
		return EBTNodeResult::Failed;

	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Enemy* enemy = Cast<AC_Enemy>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();

	if (target == nullptr)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(), enemy->GetPlayerLocation());
		LookAtRotation.Pitch = 0.0f;
		LookAtRotation.Roll = 0.0f;

		enemy->SetActorRotation(LookAtRotation);
	}
	else
	{
		enemy->GetIsHit() = false;
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Succeeded;
}

void UC_BTTaskHit::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();


	AC_Enemy* enemy = Cast<AC_Enemy>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();
}
