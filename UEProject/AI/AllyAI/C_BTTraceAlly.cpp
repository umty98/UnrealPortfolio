// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AllyAI/C_BTTraceAlly.h"
#include "AI/AllyAI/C_AllyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"
#include "Unit/C_Unit.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"

UC_BTTraceAlly::UC_BTTraceAlly()
{
	bNotifyTick = true;

	NodeName = "TraceAlly";
}

EBTNodeResult::Type UC_BTTraceAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_AllyAIController* controller = Cast<AC_AllyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_AllyWarrior* ally = Cast<AC_AllyWarrior>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();
	

	if (target == nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "executefailed");
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UC_BTTraceAlly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_AllyAIController* controller = Cast<AC_AllyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_AllyWarrior* ally = Cast<AC_AllyWarrior>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();
	/*
	if (!target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FString EnemyName = target->GetName();
	UKismetSystemLibrary::PrintString(GetWorld(), EnemyName);

	controller->MoveToActor(target, 300.0f, true, true, false, 0, true);

	if (target != nullptr)
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	*/
	if (target == nullptr)
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	float distanceToTarget = FVector::Dist(controller->GetPawn()->GetActorLocation(), target->GetActorLocation());

	if (distanceToTarget > controller->GetBehaviorRange())
	{
		controller->MoveToActor(target, 100.0f, true, true, true, 0, true);
	}
	else
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
