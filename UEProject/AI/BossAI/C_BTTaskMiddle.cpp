// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/C_BTTaskMiddle.h"
#include "AI/BossAI/C_BossAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/C_Unit.h"
#include "Unit/Boss/C_Boss.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"

UC_BTTaskMiddle::UC_BTTaskMiddle()
{
	NodeName = "AttackMiddle";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskMiddle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_BossAIController* controller = Cast<AC_BossAIController>(OwnerComp.GetOwner());
	if (!controller)
		return EBTNodeResult::Failed;

	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Boss* boss = Cast<AC_Boss>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();

	if (target == nullptr)
		return EBTNodeResult::Failed;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(boss->GetActorLocation(), target->GetActorLocation());
	rotation.Pitch = boss->GetActorRotation().Pitch;
	boss->SetActorRotation(rotation);

	if (boss->GetCanUseMiddleSkill())
	{
		boss->MiddleAttackActive();
	}

	return EBTNodeResult::InProgress;
}

void UC_BTTaskMiddle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_BossAIController* controller = Cast<AC_BossAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Boss* boss = Cast<AC_Boss>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();

	if (boss->GetIsMiddleSkillUsed())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "succeeded");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
