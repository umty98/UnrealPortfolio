// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/MousePet/C_MousePet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/PetAI/C_PetPatrolComponent.h"

AC_MousePet::AC_MousePet()
{
	TeamID = ETeamAttitude::Friendly;

	PatrolComponent = CreateDefaultSubobject<UC_PetPatrolComponent>("Patrol");
}

void AC_MousePet::BeginPlay()
{
	Super::BeginPlay();

	ChangeWeapon(EWeaponState::SWORD);
}

void AC_MousePet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanMove)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else
		GetCharacterMovement()->MaxWalkSpeed = 300;
}

void AC_MousePet::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	this->bIsFloating = false;
}

void AC_MousePet::PetFloating()
{
	this->bIsFloating = true;

}
