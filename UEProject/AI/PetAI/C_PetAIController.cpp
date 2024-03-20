// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PetAI/C_PetAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFrameWork/Actor.h"
#include "Unit/MousePet/C_MousePet.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/Enemy/C_Enemy.h"
#include "Unit/Warrior/C_Warrior.h"

AC_PetAIController::AC_PetAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	BehaviorComponent = CreateDefaultSubobject<UC_BehaviorComponent>("Behavior");

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 1200.0f;
	Sight->LoseSightRadius = 1500.0f;
	Sight->PeripheralVisionAngleDegrees = 360.0f;
	Sight->SetMaxAge(2);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void AC_PetAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AC_PetAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	FVector center = Owner->GetActorLocation();
	
	DrawDebugCircle
	(
		GetWorld(),
		center,
		Sight->SightRadius,
		300,
		FColor::Green,
		false, -1.0f, 0, 0,
		Owner->GetActorRightVector(),
		Owner->GetActorForwardVector()
	);

	center.Z += 1.0f;

	DrawDebugCircle
	(
		GetWorld(),
		center,
		BehaviorRange,
		300,
		FColor::Red,
		false, -1.0f, 0, 0,
		Owner->GetActorRightVector(),
		Owner->GetActorForwardVector()
	);
	*/



}

void AC_PetAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Owner = Cast<AC_MousePet>(InPawn);

	SetGenericTeamId(Owner->GetGenericTeamId());

	Perception->OnPerceptionUpdated.AddDynamic(this, &AC_PetAIController::OnPerceptionUpdatedPet);

	UBlackboardComponent* blackBoard = Blackboard.Get();

	UseBlackboard(Owner->GetBT()->BlackboardAsset, blackBoard);

	BehaviorComponent->SetBlackBoard(blackBoard);

	RunBehaviorTree(Owner->GetBT());
}

void AC_PetAIController::OnPerceptionUpdatedPet(const TArray<AActor*>& UpdateActors)
{
	/*
	AC_Enemy* enemy = nullptr;

	for (AActor* actor : UpdateActors)
	{
		enemy = Cast<AC_Enemy>(actor);
		
		if (enemy != nullptr)
		{
			Blackboard->SetValueAsObject(BehaviorComponent->GetEnemyKey(), enemy);
			return;
		}
	}
	*/

	UObject* currentTarget = Blackboard->GetValueAsObject(BehaviorComponent->GetEnemyKey());

	if (currentTarget != nullptr)
	{
		TArray<AActor*> perceivedActors;
		Perception->GetCurrentlyPerceivedActors(Sight->GetSenseImplementation(), perceivedActors);
		if (perceivedActors.Contains(currentTarget))
		{
			return;
		}
	}

	// 시야 범위 내에 새로운 적 탐색
	for (AActor* actor : UpdateActors)
	{
	
		AC_Enemy* enemy = Cast<AC_Enemy>(actor);
		if (enemy != nullptr && enemy != currentTarget)
		{
			// 새로운 적을 Blackboard에 설정
			//UKismetSystemLibrary::PrintString(GetWorld(), "setnewenemy");
			Blackboard->SetValueAsObject(BehaviorComponent->GetEnemyKey(), enemy);
			break;
		}
	}

}
