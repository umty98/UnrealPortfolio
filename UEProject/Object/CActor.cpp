// Fill out your copyright notice in the Description page of Project Settings.


#include "CActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACActor::ACActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	
	cube->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(L"/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'");
	
	cube->SetStaticMesh(mesh.Object);
}

// Called when the game starts or when spawned
void ACActor::BeginPlay()
{
	Super::BeginPlay();

	//UKismetSystemLibrary::PrintString(GetWorld(), L"HelloWorld");
}

// Called every frame
void ACActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

