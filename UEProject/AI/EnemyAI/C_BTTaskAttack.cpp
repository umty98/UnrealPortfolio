// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_BTTaskAttack.h"
#include "AI/EnemyAI/C_EnemyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/C_Unit.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/C_Weapon.h"
#include "Unit/Enemy/C_Enemy.h"

UC_BTTaskAttack::UC_BTTaskAttack()
{
	NodeName = "Attack";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	//UKismetSystemLibrary::PrintString(GetWorld(), "ExecuteTask");

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	if (!controller)
		return EBTNodeResult::Failed;

	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* enemy = Cast<AC_Unit>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();

	
	if (target == nullptr)
		return EBTNodeResult::Failed;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(), target->GetActorLocation());
	rotation.Pitch = enemy->GetActorRotation().Pitch;
	enemy->SetActorRotation(rotation);

	enemy->GetCanMove() = false;

	if (!enemy->GetIsDead())
	{
		if (!enemy->GetAttackDelay())
		{
			enemy->GetCurrentWeapon()->Attack();
			enemy->GetAttackDelay() = true;
		}
	}
	
	return EBTNodeResult::InProgress;
	
}

void UC_BTTaskAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* enemy = Cast<AC_Unit>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetPlayer();

	/*
	if (enemy->GetIsAttacking() == false)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "Succeeded");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	*/

	if (enemy->GetAttackDelay() == true)
	{
		//enemy->GetCanMove() = true;
		//UKismetSystemLibrary::PrintString(GetWorld(), "delaytrue");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	

	if (target == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "notarget");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
