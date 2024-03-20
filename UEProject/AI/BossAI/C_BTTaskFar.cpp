// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/C_BTTaskFar.h"
#include "AI/BossAI/C_BossAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/C_Unit.h"
#include "Unit/Boss/C_Boss.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"

UC_BTTaskFar::UC_BTTaskFar()
{
	NodeName = "AttackFar";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskFar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	if (boss->GetHp() > boss->GetMaxHp() / 2)
	{
		if (boss->GetCanUseSpearSkill())
		{
			boss->SetWarriorLocation(target->GetActorLocation());
			boss->FallingSpearActive();
		}

	}
	else
	{
		if (boss->GetCanUseFollowingSkill())
		{
			boss->FollowingSpearActive();
		}
	}

	/*
	
	*/


	return EBTNodeResult::InProgress;
}

void UC_BTTaskFar::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_BossAIController* controller = Cast<AC_BossAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Boss* boss = Cast<AC_Boss>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();



	if (boss->GetIsFallingSpearSkillUsed())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "fallingsucceeded");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	

	if (boss->GetIsFollowingSkillUsed())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "followingsucceeded");;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	
	

}
