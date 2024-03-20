// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAI/C_BossAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "Unit/Boss/C_Boss.h"
#include "Unit/Warrior/C_Warrior.h"

AC_BossAIController::AC_BossAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	BehaviorComponent = CreateDefaultSubobject<UC_BehaviorComponent>("Behavior");

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 4800.0f;
	Sight->LoseSightRadius = 5000.0f;
	Sight->PeripheralVisionAngleDegrees = 360.0f;
	Sight->SetMaxAge(2);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void AC_BossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AC_BossAIController::Tick(float DeltaTime)
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
		BehaviorRangeNear,
		300,
		FColor::Red,
		false, -1.0f, 0, 0,
		Owner->GetActorRightVector(),
		Owner->GetActorForwardVector()
	);

	DrawDebugCircle
	(
		GetWorld(),
		center,
		BehaviorRangeMiddle,
		300,
		FColor::Blue,
		false, -1.0f, 0, 0,
		Owner->GetActorRightVector(),
		Owner->GetActorForwardVector()
	);

	DrawDebugCircle
	(
		GetWorld(),
		center,
		BehaviorRangeFar,
		300,
		FColor::Emerald,
		false, -1.0f, 0, 0,
		Owner->GetActorRightVector(),
		Owner->GetActorForwardVector()
	);
	*/
}

void AC_BossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Owner = Cast<AC_Boss>(InPawn);

	SetGenericTeamId(Owner->GetGenericTeamId());

	Perception->OnPerceptionUpdated.AddDynamic(this, &AC_BossAIController::OnPerceptionUpdated);

	UBlackboardComponent* blackBoard = Blackboard.Get();

	UseBlackboard(Owner->GetBT()->BlackboardAsset, blackBoard);

	BehaviorComponent->SetBlackBoard(blackBoard);

	RunBehaviorTree(Owner->GetBT());
}

void AC_BossAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdateActors)
{
	AC_Warrior* player = nullptr;

	for (AActor* actor : UpdateActors)
	{
		player = Cast<AC_Warrior>(actor);

		if (player != nullptr)
		{
			Blackboard->SetValueAsObject(BehaviorComponent->GetPlayerKey(), player);
			return;
		}

	}

}
