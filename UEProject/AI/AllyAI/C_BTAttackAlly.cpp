// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AllyAI/C_BTAttackAlly.h"
#include "AI/AllyAI/C_AllyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"
#include "Unit/C_Unit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/C_Weapon.h"
#include "Kismet/KismetMathLibrary.h"

UC_BTAttackAlly::UC_BTAttackAlly()
{
    NodeName = "AttackAlly";

    bNotifyTick = true;
}

EBTNodeResult::Type UC_BTAttackAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_AllyAIController* controller = Cast<AC_AllyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_AllyWarrior* ally = Cast<AC_AllyWarrior>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();

	if (target == nullptr || target->GetHp() <= 0)
		return EBTNodeResult::Failed;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(ally->GetActorLocation(), target->GetActorLocation());
	rotation.Pitch = ally->GetActorRotation().Pitch;
	ally->SetActorRotation(rotation);

	ally->GetCanMove() = false;

	float distance = ally->GetDistanceTo(target);

	if (!ally->GetIsDead()&&target->GetHp()>0)
	{
		if (!ally->GetAttackDelay())
		{
			ally->GetCurrentWeapon()->Attack();
			ally->GetAttackDelay() = true;
		}
	}

	return EBTNodeResult::InProgress;

}

void UC_BTAttackAlly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_AllyAIController* controller = Cast<AC_AllyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_AllyWarrior* ally = Cast<AC_AllyWarrior>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();

	if (ally->GetAttackDelay() == true)
	{
		//enemy->GetCanMove() = true;
		//UKismetSystemLibrary::PrintString(GetWorld(), "delaytrue");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	float distance = ally->GetDistanceTo(target);

	
	if (distance >= controller->GetBehaviorRange())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "succeed");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	

	
	if (target->GetHp() <= 0)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "enemyhp0");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	

	if (target == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "notarget");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
