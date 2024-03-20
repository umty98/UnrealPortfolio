// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_BTMelee.h"
#include "AI/EnemyAI/C_EnemyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/C_Unit.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Object/C_Weapon.h"

UC_BTMelee::UC_BTMelee()
{
	NodeName = "Melee";
}

void UC_BTMelee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AC_EnemyAIController* controller = Cast<AC_EnemyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* enemy = Cast<AC_Unit>(controller->GetPawn());
	AC_Unit* target = nullptr;

	target = behaviorComponent->GetPlayer();

	/*
	if (target != nullptr)
	{
		FString TargetName = target->GetName();
		UKismetSystemLibrary::PrintString(this, TargetName);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "NOTARGET");
	}
	*/

	if (enemy->GetHp() <= 0)
	{
		behaviorComponent->ChangeType(EBehaviorType::DEATH);
		OwnerComp.StopTree(EBTStopMode::Safe);
		//UKismetSystemLibrary::PrintString(GetWorld(), "death");

		return;
	}

	if (enemy->GetIsHit())
	{
		controller->StopMovement();
		behaviorComponent->ChangeType(EBehaviorType::HIT);
		//UKismetSystemLibrary::PrintString(GetWorld(), "HIT");

		return;
	}


	float distance = enemy->GetDistanceTo(target);
	
	if (target == nullptr)
	{
		//enemy->GetCanMove() = true;
		behaviorComponent->ChangeType(EBehaviorType::PATROL);

		//UKismetSystemLibrary::PrintString(GetWorld(), "patrol");
		return;
	}


	if (distance < controller->GetBehaviorRange())
	{
		//enemy->GetCanMove() = false;
		behaviorComponent->ChangeType(EBehaviorType::ATTACK);
		//enemy->GetCurrentWeapon()->Attack();

		//UKismetSystemLibrary::PrintString(GetWorld(), "attack");
		return;
	}


	if (distance < controller->GetSight()->SightRadius)
	{
		//enemy->GetCanMove() = true;
		behaviorComponent->ChangeType(EBehaviorType::TRACE);

		//UKismetSystemLibrary::PrintString(GetWorld(), "trace");
		return;
	}

	behaviorComponent->ChangeType(EBehaviorType::PATROL);
	behaviorComponent->SetPlayer(nullptr);
}
