// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AllyAI/C_BTAlly.h"
#include "AI/AllyAI/C_AllyAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"
#include "Unit/C_Unit.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Object/C_Weapon.h"

UC_BTAlly::UC_BTAlly()
{
	NodeName = "Ally";
}

void UC_BTAlly::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AC_AllyAIController* controller = Cast<AC_AllyAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_AllyWarrior* ally = Cast<AC_AllyWarrior>(controller->GetPawn());
	AC_Unit* target = behaviorComponent->GetEnemy();


	if (!ally->GetCanAttack())
	{
		return;
	}

	/*
	if (target != nullptr)
	{
		FString EnemyName = target->GetName();
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("New target detected: %s"), *EnemyName), true, true, FLinearColor::White, 5.0f);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "null");
	}
	*/


	
	if (ally->GetHp() <= 0)
	{
		behaviorComponent->ChangeType(EBehaviorType::DEATH);
		OwnerComp.StopTree(EBTStopMode::Safe);
		return;
	}
	

	if (target == nullptr)
	{
		behaviorComponent->ChangeType(EBehaviorType::WAIT);
		return;
	}

	float distance = ally->GetDistanceTo(target);
	/*
	if (target != nullptr)
	{
		UKismetSystemLibrary::PrintString(this, "Find");
		behaviorComponent->ChangeType(EBehaviorType::TRACE);
		return;
	}
	*/

	if (distance >= controller->GetBehaviorRange())
	{
		//UKismetSystemLibrary::PrintString(this, "Find");
		behaviorComponent->ChangeType(EBehaviorType::TRACE);
		return;
	}

	if (distance < controller->GetBehaviorRange())
	{
		//UKismetSystemLibrary::PrintString(this, "Attack");
		behaviorComponent->ChangeType(EBehaviorType::ATTACK);

		return;
	}





	behaviorComponent->ChangeType(EBehaviorType::WAIT);

}
