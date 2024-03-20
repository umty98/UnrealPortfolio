// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PetAI/C_BTTaskTracePet.h"
#include "AI/PetAI/C_PetAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/MousePet/C_MousePet.h"
#include "Unit/C_Unit.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"


UC_BTTaskTracePet::UC_BTTaskTracePet()
{
	bNotifyTick = true;

	NodeName = "TracePet";
}

EBTNodeResult::Type UC_BTTaskTracePet::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_PetAIController* controller = Cast<AC_PetAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_MousePet* pet = Cast<AC_MousePet>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();

	if (target == nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "executefailed");
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UC_BTTaskTracePet::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_PetAIController* controller = Cast<AC_PetAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_MousePet* pet = Cast<AC_MousePet>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();

	/*
	if (target == nullptr)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "targetnull");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	float distanceToTarget;

	if (target != nullptr)
	{
		distanceToTarget = FVector::Dist(controller->GetPawn()->GetActorLocation(), target->GetActorLocation());
		controller->MoveToActor(target, 50.0f, false, true, true, 0, true);
	}
	else
	{
		pet->GetCanMove() = false;
	}


	if (distanceToTarget <= controller->GetBehaviorRange())
	{
		pet->GetCanMove() = false;
		//UKismetSystemLibrary::PrintString(GetWorld(), "inbehaviorrange");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
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
		controller->MoveToActor(target, 50.0f, false, true, true, 0, true);
	}
	else
	{
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
