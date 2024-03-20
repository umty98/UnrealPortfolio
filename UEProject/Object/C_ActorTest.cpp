// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_ActorTest.h"

// Sets default values
AC_ActorTest::AC_ActorTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_ActorTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_ActorTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

