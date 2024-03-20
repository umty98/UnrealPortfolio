// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PetAI/C_BTTaskPatrolPet.h"
#include "AI/PetAI/C_PetAIController.h"
#include "Unit/MousePet/C_MousePet.h"
#include "AI/PetAI/C_PetPatrolComponent.h"
#include "Navigation/PathFollowingComponent.h"

UC_BTTaskPatrolPet::UC_BTTaskPatrolPet()
{
	bNotifyTick = true;

	NodeName = "PatrolPet";
}

EBTNodeResult::Type UC_BTTaskPatrolPet::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AC_PetAIController* controller = Cast<AC_PetAIController>(OwnerComp.GetOwner());

	AC_MousePet* pet = Cast<AC_MousePet>(controller->GetPawn());

	UC_PetPatrolComponent* patrol = pet->GetPatrolComponent();

	FVector location;
	float acceptanceRadius;

	if (patrol->GetMoveTo(location, acceptanceRadius) == false)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UC_BTTaskPatrolPet::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AC_PetAIController* controller = Cast<AC_PetAIController>(OwnerComp.GetOwner());

	AC_MousePet* pet = Cast<AC_MousePet>(controller->GetPawn());

	UC_PetPatrolComponent* patrol = pet->GetPatrolComponent();

	FVector location;
	float acceptanceRadius;

	patrol->GetMoveTo(location, acceptanceRadius);

	EPathFollowingRequestResult::Type typepet = controller->MoveToLocation(location, acceptanceRadius, false);

	if (typepet == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		//UKismetSystemLibrary::PrintString(GetWorld(), "failed");
		return;
	}
	
	/*
	float DistanceToTarget = FVector::Dist(pet->GetActorLocation(), location);

	FString DistanceMessage = FString::Printf(TEXT("Distance to Target: %f"), DistanceToTarget);
	UKismetSystemLibrary::PrintString(GetWorld(), DistanceMessage);

	if (DistanceToTarget <= acceptanceRadius)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("InRange"));
		patrol->GetHasRandomLocation() = false; // 다음 목표 위치를 위해 상태를 리셋합니다.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	else
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("OutRange"));
	}
	*/
	

	if (typepet == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "succeeded");
		patrol->GetHasRandomLocation() = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
