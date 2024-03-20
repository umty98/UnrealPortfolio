// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_BTTaskPatrol.h"
#include "AI/EnemyAI/C_EnemyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/Enemy/C_Enemy.h"
#include "AI/EnemyAI/C_PatrolComponent.h"
#include "Unit/C_Unit.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"


UC_BTTaskPatrol::UC_BTTaskPatrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());

	AC_Enemy* enemy = Cast<AC_Enemy>(controller->GetPawn());

	UC_PatrolComponent* patrol = enemy->GetPatrolComponent();

	FVector location;
	float acceptanceRadius;

	if (patrol->GetMoveTo(location, acceptanceRadius) == false)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
	
}

void UC_BTTaskPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Enemy* enemy = Cast<AC_Enemy>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();

	UC_PatrolComponent* patrol = enemy->GetPatrolComponent();

	FVector location;
	float acceptanceRadius;

	patrol->GetMoveTo(location, acceptanceRadius);

	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, acceptanceRadius, false);

	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	if (target != nullptr)
	{
		//patrol->GetHasRandomLocation() = false;
		//UKismetSystemLibrary::PrintString(GetWorld(), "succeeded");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	if (enemy->GetIsHit())
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//UKismetSystemLibrary::PrintString(GetWorld(), "succeeded");
	}
	
	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		patrol->GetHasRandomLocation() = false;
		//UKismetSystemLibrary::PrintString(GetWorld(), "succeeded");
		patrol->UpdateNextIndex();
	}
}
