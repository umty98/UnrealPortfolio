// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PetAI/C_BTPet.h"
#include "AI/PetAI/C_PetAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/C_Unit.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Object/C_Weapon.h"

UC_BTPet::UC_BTPet()
{
	NodeName = "Pet";
}

void UC_BTPet::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AC_PetAIController* controller = Cast<AC_PetAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Unit* pet = Cast<AC_Unit>(controller->GetPawn());
	//AC_Unit* target = nullptr;
	AC_Unit* target = behaviorComponent->GetEnemy();
	//target = behaviorComponent->GetEnemy();
	
	/*
	if (target != nullptr)
	{
		FString TargetName = target->GetName();
		UKismetSystemLibrary::PrintString(this, TargetName);
	}
	*/

	if (target == nullptr)
	{
		behaviorComponent->ChangeType(EBehaviorType::WAIT);
		return;
	}
	
	float distance = pet->GetDistanceTo(target);

	if (distance >= controller->GetSight()->LoseSightRadius)
	{
		behaviorComponent->SetEnemy(nullptr);
	}



	if (distance < controller->GetBehaviorRange())
	{
		behaviorComponent->ChangeType(EBehaviorType::ATTACK);
		return;
	}


	if (distance < controller->GetSight()->SightRadius)
	{
		behaviorComponent->ChangeType(EBehaviorType::TRACE);
		return;
	}

	behaviorComponent->ChangeType(EBehaviorType::WAIT);
	//behaviorComponent->SetEnemy(nullptr);
}
