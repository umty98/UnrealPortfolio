// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_PetSpawn.h"
#include "Components/DecalComponent.h"

// Sets default values
AC_PetSpawn::AC_PetSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);

	DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f)); 
	DecalComponent->DecalSize = FVector(100.0f, 100.0f, 100.0f);

}

// Called when the game starts or when spawned
void AC_PetSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_PetSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

