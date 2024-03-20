// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_Orb.h"
#include "Unit/C_Unit.h"

AC_Orb::AC_Orb()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC_Orb::BeginPlay()
{
	Super::BeginPlay();
}

void AC_Orb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float RotationSpeed = 200.0f; 

    float DeltaRotation = RotationSpeed * DeltaTime;

    FRotator NewRotation = FRotator(0, DeltaRotation, 0);

    AddActorLocalRotation(NewRotation);
}

void AC_Orb::ActiveShield()
{
	Owner->PlayAnimMontage(SwordComboData[0]->Animation);
}

void AC_Orb::ActiveMiddleAttack()
{
	Owner->PlayAnimMontage(SwordComboData[1]->Animation, 0.6f);
}

void AC_Orb::ActiveFallingSpear()
{
	Owner->PlayAnimMontage(SwordComboData[2]->Animation);
}

void AC_Orb::ActiveFollowingSpear()
{
	Owner->PlayAnimMontage(SwordComboData[3]->Animation);
}
