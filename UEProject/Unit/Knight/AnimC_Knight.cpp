// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Knight/AnimC_Knight.h"
#include "C_Knight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/C_FeetComponent.h"

void UAnimC_Knight::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr)
		return;

	FRotator rotator(0, Owner->GetControlRotation().Yaw, 0);

	Direction = CalculateDirection(Owner->GetVelocity().GetSafeNormal2D(), rotator);

	bFlip = Cast<AC_Knight>(Owner)->GetIsFlip();

	UC_FeetComponent* feetComponent = Cast<UC_FeetComponent>(Owner->GetComponentByClass(UC_FeetComponent::StaticClass()));

	if (feetComponent)
	{

		FeetData = feetComponent->GetData();
	}
}
