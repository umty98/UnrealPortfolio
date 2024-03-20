// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_PatrolComponent.h"
#include "AI/EnemyAI/C_PatrolPath.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "Unit/Enemy/C_Enemy.h"

UC_PatrolComponent::UC_PatrolComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;


}

void UC_PatrolComponent::BeginPlay()
{
	Super::BeginPlay();

}


bool UC_PatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius)
{
	OutLocation = FVector::ZeroVector;

	OutAcceptanceRadius = AcceptanceRadius;
	/*
	if (Path == nullptr)
		return false;

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return true;
	*/
	
	if (Path)
	{
		bHasValidRandomLocation = false;
		OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
		return true;
	}
	else 
	{
		AC_Enemy* EnemyOwner = Cast<AC_Enemy>(GetOwner());

		if (EnemyOwner->GetIsLeader())
		{
			if (!bHasValidRandomLocation)
			{
				UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
				if (NavSys)
				{
					FVector StartLocation = EnemyOwner->GetLeaderStartLocation();
					FNavLocation RandomLocation;
					if (NavSys->GetRandomPointInNavigableRadius(GetOwner()->GetActorLocation(), 4000.0f, RandomLocation))
					{
						LastRandomLocation = RandomLocation.Location;
						bHasValidRandomLocation = true;
					}
				}
			}

			OutLocation = LastRandomLocation;
			return bHasValidRandomLocation;
		}
		else if (EnemyOwner->LeaderEnemy)
		{
			/*
			FVector LeaderLocation = EnemyOwner->LeaderEnemy->GetActorLocation();
			FVector Direction = (EnemyOwner->GetActorLocation() - LeaderLocation).GetSafeNormal();
			OutLocation = LeaderLocation + Direction * 100.0f; // FollowDistance는 Leader와 Follower 사이의 거리입니다.
			return true;
			*/
			FVector RelativePosition = EnemyOwner->RelativePositionToLeader;
			OutLocation = EnemyOwner->LeaderEnemy->GetActorLocation() + RelativePosition;
			return true;
		}
	}

	return false;

}

void UC_PatrolComponent::UpdateNextIndex()
{
	if (Path == nullptr)
		return;

	int32 count = Path->GetSpline()->GetNumberOfSplinePoints();

	if (bIsReverse)
	{
		if (Index > 0)
		{
			Index--;
			return;
		}

		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}

		Index = 1;

		bIsReverse = false;
		return;
	}

	if (Index < count - 1)
	{
		Index++;
		return;
	}

	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	Index = count - 2;
	bIsReverse = true;
}

