// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/C_BTBoss.h"
#include "AI/BossAI/C_BossAIController.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/C_Unit.h"
#include "Unit/Boss/C_Boss.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Object/C_Weapon.h"

UC_BTBoss::UC_BTBoss()
{
	NodeName = "Boss";
}

void UC_BTBoss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AC_BossAIController* controller = Cast<AC_BossAIController>(OwnerComp.GetOwner());
	UC_BehaviorComponent* behaviorComponent = controller->GetBehaviorComponent();

	AC_Boss* boss = Cast<AC_Boss>(controller->GetPawn());
	AC_Unit* target = nullptr;

	target = behaviorComponent->GetPlayer();

	/*
	if (target != nullptr)
	{
		FString TargetName = target->GetName();
		UKismetSystemLibrary::PrintString(this, TargetName);
	}
	*/

	if (!boss->GetIsCenter())
	{
		behaviorComponent->ChangeType(EBehaviorType::TRACE);
		return;
	}

	if (target == nullptr)
	{
		behaviorComponent->ChangeType(EBehaviorType::WAIT);
		return;
	}

	float distance = boss->GetDistanceTo(target);

	if (boss->GetCanUseShieldSkill() && distance < controller->GetBehaviorRangeNear())
	{
		behaviorComponent->ChangeType(EBehaviorType::ATTACKNEAR);
		//UKismetSystemLibrary::PrintString(GetWorld(), "near");
		return;
	}


	if (boss->GetCanUseMiddleSkill() && distance < controller->GetBehaviorRangeMiddle())
	{
		behaviorComponent->ChangeType(EBehaviorType::ATTACKMIDDLE);
		//UKismetSystemLibrary::PrintString(GetWorld(), "middle");
		return;
	}

	if (boss->GetCanUseFarSkill() && distance < controller->GetBehaviorRangeFar())
	{
		behaviorComponent->ChangeType(EBehaviorType::ATTACKFAR);
		//UKismetSystemLibrary::PrintString(GetWorld(), "far");
		return;
	}


	behaviorComponent->ChangeType(EBehaviorType::WAIT);
	





}
