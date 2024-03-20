// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/AllyWarrior/C_AllyWarrior.h"
#include "GameFramework/CharacterMovementComponent.h"

AC_AllyWarrior::AC_AllyWarrior()
{
	TeamID = ETeamAttitude::Friendly;
}

void AC_AllyWarrior::BeginPlay()
{
	Super::BeginPlay();

	//ChangeWeapon(EWeaponState::SWORD);
}

void AC_AllyWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanMove)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else
		GetCharacterMovement()->MaxWalkSpeed = 400;
}
