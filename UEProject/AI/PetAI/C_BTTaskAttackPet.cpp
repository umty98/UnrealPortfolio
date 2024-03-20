// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PetAI/C_BTTaskAttackPet.h"
#include "AI/PetAI/C_PetAIController.h"
#include "Unit/MousePet/C_MousePet.h"
#include "Unit/Enemy/C_Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Object/C_Weapon.h"

UC_BTTaskAttackPet::UC_BTTaskAttackPet()
{
	NodeName = "AttackPet";

	bNotifyTick = true;
}

EBTNodeResult::Type UC_BTTaskAttackPet::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	//UKismetSystemLibrary::PrintString(GetWorld(), "ExecuteTask");

	AC_PetAIController* controller = Cast<AC_PetAIController>(OwnerComp.GetOwner());
	if (!controller)
		return EBTNodeResult::Failed;

	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* pet = Cast<AC_Unit>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();


	if (target == nullptr)
		return EBTNodeResult::Failed;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(pet->GetActorLocation(), target->GetActorLocation());
	rotation.Pitch = pet->GetActorRotation().Pitch;
	pet->SetActorRotation(rotation);

	//if (!pet->GetIsDead())
	pet->GetCanMove() = false;

	if (!pet->GetAttackDelay())
	{
		pet->GetCurrentWeapon()->Attack();
	}

	return EBTNodeResult::InProgress;

}

void UC_BTTaskAttackPet::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_PetAIController* controller = Cast<AC_PetAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* pet = Cast<AC_Unit>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();

	
	if (pet->GetIsAttacking() == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
	/*
	if (pet->GetAttackDelay() == true)
	{
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	*/


	if (target == nullptr)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "notarget");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

