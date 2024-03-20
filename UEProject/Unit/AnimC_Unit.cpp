// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/AnimC_Unit.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimC_Unit::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<AC_Unit>(TryGetPawnOwner());
}

void UAnimC_Unit::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr)
		return;

	Speed = Owner->GetVelocity().Size2D();

	bFalling = Owner->GetCharacterMovement()->IsFalling();

	bStunned = Owner->GetIsStun();

	bCanStandUp = Owner->GetCanStandUp();

	WeaponType = Owner->GetWeaponType();

	CharacterState = Owner->GetCharacterType();

	bCanBlock = Owner->GetIsBlocking();
}
