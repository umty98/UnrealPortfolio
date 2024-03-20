// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/C_BTTaskMoveToCenter.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "AI/BossAI/C_BossAIController.h"
#include "Unit/C_Unit.h"
#include "Unit/Boss/C_Boss.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

UC_BTTaskMoveToCenter::UC_BTTaskMoveToCenter()
{
	NodeName = "MoveToCenter";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskMoveToCenter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_BossAIController* controller = Cast<AC_BossAIController>(OwnerComp.GetOwner());
	if (!controller)
		return EBTNodeResult::Failed;

	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Boss* boss = Cast<AC_Boss>(controller->GetPawn());
	

	return EBTNodeResult::InProgress;
}

void UC_BTTaskMoveToCenter::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_BossAIController* controller = Cast<AC_BossAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Boss* boss = Cast<AC_Boss>(controller->GetPawn());

	FVector CenterLocation(-4860, 14180.0, boss->GetActorLocation().Z);
	if (FVector::Dist(boss->GetActorLocation(), CenterLocation) < 100)
	{

		//UKismetSystemLibrary::PrintString(GetWorld(), "Stop");
		boss->GetIsCenter() = true;
		controller->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Move");
		controller->MoveToLocation(CenterLocation); 
	}


}
