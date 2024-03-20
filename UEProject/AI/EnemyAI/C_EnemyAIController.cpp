// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Actor.h"
#include "Unit/Enemy/C_Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/Warrior/C_Warrior.h"
#include "Unit/MousePet/C_MousePet.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"


AC_EnemyAIController::AC_EnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	BehaviorComponent = CreateDefaultSubobject<UC_BehaviorComponent>("Behavior");

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 1200.0f;
	Sight->LoseSightRadius = 1500.0f;
	Sight->PeripheralVisionAngleDegrees = 120.0f;
	Sight->SetMaxAge(2);
	Sight->DetectionByAffiliation.bDetectEnemies    = true;
	Sight->DetectionByAffiliation.bDetectNeutrals   = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());


}

void AC_EnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AC_EnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	FVector center = Owner->GetActorLocation();

	if (Owner->GetIsHit())
	{
		Sight->SightRadius = 800.0f;
		Sight->LoseSightRadius = 1000.0f;
		Sight->PeripheralVisionAngleDegrees = 120.0f;
	}
	else
	{
		Sight->SightRadius = 600.0f;
		Sight->LoseSightRadius = 800.0f;
		Sight->PeripheralVisionAngleDegrees = 90.0f;
	}
	

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

void AC_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Owner = Cast<AC_Enemy>(InPawn);

	SetGenericTeamId(Owner->GetGenericTeamId());

	Perception->OnPerceptionUpdated.AddDynamic(this, &AC_EnemyAIController::OnPerceptionUpdated);

	UBlackboardComponent* blackBoard = Blackboard.Get();

	UseBlackboard(Owner->GetBT()->BlackboardAsset, blackBoard);

	BehaviorComponent->SetBlackBoard(blackBoard);

	RunBehaviorTree(Owner->GetBT());
}

void AC_EnemyAIController::ChangeSight(float sightRadius, float loseSightRadius, float PVAD)
{
	Sight->SightRadius = sightRadius;
	Sight->LoseSightRadius = loseSightRadius;
	Sight->PeripheralVisionAngleDegrees = PVAD;
}

void AC_EnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdateActors)
{
	UObject* currentTarget = Blackboard->GetValueAsObject(BehaviorComponent->GetPlayerKey());

	if (currentTarget != nullptr)
	{
		TArray<AActor*> perceivedActors;
		Perception->GetCurrentlyPerceivedActors(Sight->GetSenseImplementation(), perceivedActors);
		if (perceivedActors.Contains(currentTarget))
		{
			return;
		}
	}

	for (AActor* actor : UpdateActors)
	{
		AC_Warrior* player = Cast<AC_Warrior>(actor);
		AC_MousePet* mousePet = Cast<AC_MousePet>(actor);
		AC_AllyWarrior* allyWarrior = Cast<AC_AllyWarrior>(actor);

		if (player != nullptr && player != currentTarget)
		{
			Blackboard->SetValueAsObject(BehaviorComponent->GetPlayerKey(), player);
			break;
		}

		if (mousePet != nullptr && mousePet != currentTarget)
		{
			Blackboard->SetValueAsObject(BehaviorComponent->GetPlayerKey(), mousePet);
			break;
		}

		if (allyWarrior != nullptr && allyWarrior != currentTarget)
		{
			Blackboard->SetValueAsObject(BehaviorComponent->GetPlayerKey(), allyWarrior);
			break;
		}

		
	}

}
