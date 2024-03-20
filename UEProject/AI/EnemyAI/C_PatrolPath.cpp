// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_PatrolPath.h"
#include "Components/SplineComponent.h"


AC_PatrolPath::AC_PatrolPath()
{
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(RootComponent);
}

void AC_PatrolPath::BeginPlay()
{
	Super::BeginPlay();

	Spline->SetClosedLoop(bIsLoop);
	
}

void AC_PatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

