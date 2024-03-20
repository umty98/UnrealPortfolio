// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AllyAI/C_AllyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFrameWork/Actor.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/Enemy/C_Enemy.h"

AC_AllyAIController::AC_AllyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	BehaviorComponent = CreateDefaultSubobject<UC_BehaviorComponent>("Behavior");

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = MAX_FLT;
	Sight->LoseSightRadius = MAX_FLT;
	Sight->PeripheralVisionAngleDegrees = 360.0f;
	Sight->SetMaxAge(2);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void AC_AllyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AC_AllyAIController::Tick(float DeltaTime)
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
	/////

	UObject* currentTarget = Blackboard->GetValueAsObject(BehaviorComponent->GetEnemyKey());
	AC_Enemy* closestEnemy = Cast<AC_Enemy>(currentTarget);

	float closestDistance = MAX_FLT;

	if (closestEnemy&&closestEnemy->GetHp()>0)
	{
		closestDistance = FVector::Dist(closestEnemy->GetActorLocation(), GetPawn()->GetActorLocation());
	}
	else
	{
		closestEnemy = nullptr;
	}

	TArray<AActor*> perceivedActors;
	Perception->GetCurrentlyPerceivedActors(Sight->GetSenseImplementation(), perceivedActors);

	for (AActor* actor : perceivedActors)
	{
		AC_Enemy* enemy = Cast<AC_Enemy>(actor);
		if (enemy && enemy->GetHp() > 0)
		{
			float distance = FVector::Dist(enemy->GetActorLocation(), GetPawn()->GetActorLocation());
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestEnemy = enemy;
			}
		}
	}

	if (closestEnemy != nullptr && closestEnemy != currentTarget)
	{
		Blackboard->SetValueAsObject(BehaviorComponent->GetEnemyKey(), closestEnemy);
	}
}

void AC_AllyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Owner = Cast<AC_AllyWarrior>(InPawn);

	SetGenericTeamId(Owner->GetGenericTeamId());

	Perception->OnPerceptionUpdated.AddDynamic(this, &AC_AllyAIController::OnPerceptionUpdatedPet);

	UBlackboardComponent* blackBoard = Blackboard.Get();

	UseBlackboard(Owner->GetBT()->BlackboardAsset, blackBoard);

	BehaviorComponent->SetBlackBoard(blackBoard);

	RunBehaviorTree(Owner->GetBT());
}

void AC_AllyAIController::OnPerceptionUpdatedPet(const TArray<AActor*>& UpdateActors)
{
	UObject* currentTarget = Blackboard->GetValueAsObject(BehaviorComponent->GetEnemyKey());
	AC_Enemy* closestEnemy = Cast<AC_Enemy>(currentTarget);

	float closestDistance = MAX_FLT;

	if (closestEnemy)
	{
		closestDistance = FVector::Dist(closestEnemy->GetActorLocation(), GetPawn()->GetActorLocation());
	}

	TArray<AActor*> perceivedActors;
	Perception->GetCurrentlyPerceivedActors(Sight->GetSenseImplementation(), perceivedActors);

	for (AActor* actor : perceivedActors)
	{
		AC_Enemy* enemy = Cast<AC_Enemy>(actor);
		if (enemy && enemy != currentTarget)
		{
			float distance = FVector::Dist(enemy->GetActorLocation(), GetPawn()->GetActorLocation());
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestEnemy = enemy;
			}
		}
	}

	if (closestEnemy != nullptr && closestEnemy != currentTarget)
	{
		Blackboard->SetValueAsObject(BehaviorComponent->GetEnemyKey(), closestEnemy);
	}
	
}

TArray<AActor*> AC_AllyAIController::GetPerceivedEnemies()
{
	TArray<AActor*> PerceivedActors;
	TArray<AActor*> Enemies;

	if (Perception)
	{
		Perception->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);

		for (AActor* Actor : PerceivedActors)
		{
			AC_Enemy* enemy = Cast<AC_Enemy>(Actor);
			if (enemy && enemy->GetHp()>0) 
			{
				UKismetSystemLibrary::PrintString(this, "Findperactor");
				Enemies.Add(enemy);
			}
		}
	}

	return Enemies;
}
