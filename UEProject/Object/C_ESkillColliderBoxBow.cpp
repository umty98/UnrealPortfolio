// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_ESkillColliderBoxBow.h"

// Sets default values
AC_ESkillColliderBoxBow::AC_ESkillColliderBoxBow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionProfileName("Trigger");
}

// Called when the game starts or when spawned
void AC_ESkillColliderBoxBow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_ESkillColliderBoxBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

