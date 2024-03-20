// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnArea/C_EnemySpawnArea.h"
#include "Components/BoxComponent.h"
#include "AI/EnemyAI/C_EnemyAIController.h"
#include "Unit/Enemy/C_Enemy.h"

AC_EnemySpawnArea::AC_EnemySpawnArea()
{
	PrimaryActorTick.bCanEverTick = true;

	ColliderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = ColliderBox;
	ColliderBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ColliderBox->InitBoxExtent(FVector(300.0f, 300.0f, 150.0f));
	ColliderBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ColliderBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ColliderBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AC_EnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();

	CreateEnemies();
	//SpawnEnemy();

}

void AC_EnemySpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_EnemySpawnArea::CreateEnemies()
{

	FVector Origin = GetActorLocation(); 
	
	TArray<FVector> Positions = {
	  FVector(0, 0, 0),         // Áß¾Ó
	  FVector(-100, -150, 0),   // ¿ÞÂÊ
	  FVector(-100, 150, 0),    // ¿À¸¥ÂÊ
	  FVector(-250, -300, 0),   // ´õ ¿ÞÂÊ
	  FVector(-250, 300, 0)     // ´õ ¿À¸¥ÂÊ
	};

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	for (int32 i = 0; i < Positions.Num(); ++i)
	{
		FVector SpawnLocation = Origin + Positions[i];
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		AC_Enemy* SpawnedEnemy = GetWorld()->SpawnActor<AC_Enemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedEnemy)
		{
			if (i == 0)
			{
				LeaderEnemy = SpawnedEnemy;
				SpawnedEnemy->GetIsLeader() = true;
			}
			else
			{
				SpawnedEnemy->GetIsLeader() = false;
				SpawnedEnemy->SetLeader(LeaderEnemy);
				SpawnedEnemy->RelativePositionToLeader = SpawnLocation - LeaderEnemy->GetActorLocation();
			}

			auto AIController = GetWorld()->SpawnActor<AC_EnemyAIController>(AC_EnemyAIController::StaticClass());
			if (AIController)
			{
				AIController->Possess(SpawnedEnemy);
			}
		}
	}

}

void AC_EnemySpawnArea::SpawnEnemy()
{
	FVector Origin = GetActorLocation();
	FVector BoxCenter = ColliderBox->Bounds.Origin;
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AC_Enemy* SpawnedEnemy = GetWorld()->SpawnActor<AC_Enemy>(EnemyClass, BoxCenter, SpawnRotation, SpawnParams);

	if (SpawnedEnemy)
	{
		auto AIController = GetWorld()->SpawnActor<AC_EnemyAIController>(AC_EnemyAIController::StaticClass());
		if (AIController)
		{
			AIController->Possess(SpawnedEnemy);
		}
	}

	Enemies.Add(SpawnedEnemy);
}

