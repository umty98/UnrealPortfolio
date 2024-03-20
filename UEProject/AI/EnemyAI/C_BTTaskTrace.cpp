// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_BTTaskTrace.h"
#include "AI/EnemyAI/C_EnemyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/C_Unit.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

UC_BTTaskTrace::UC_BTTaskTrace()
{
	NodeName = "Trace";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	if (!controller)
		return EBTNodeResult::Failed;

	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* enemy = Cast<AC_Unit>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();


	if (target == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "executefailed");
		return EBTNodeResult::Failed;
	}


	return EBTNodeResult::InProgress;
}

void UC_BTTaskTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* enemy = Cast<AC_Unit>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();

	
	if (target == nullptr)
	{
		controller->StopMovement();

		//UKismetSystemLibrary::PrintString(GetWorld(), "stopmovement");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//return;
	}
	else
	{
		float distanceToTarget = FVector::Dist(controller->GetPawn()->GetActorLocation(), target->GetActorLocation());



		/*
		FString targetLocationString = target->GetActorLocation().ToString();

		FString distanceString = FString::Printf(TEXT("Distance to Target: %f"), distanceToTarget);

		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Target Location: %s, %s"), *targetLocationString, *distanceString));
		*/

		if (distanceToTarget > controller->GetBehaviorRange())
		{
			//distanceToTarget = FVector::Dist(controller->GetPawn()->GetActorLocation(), target->GetActorLocation());
			controller->MoveToActor(target, 100.0f, false, true, true, 0, true);
		}
		else
		{
			controller->StopMovement();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

	
	
}
