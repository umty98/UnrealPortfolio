// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Warrior/AnimC_Warrior.h"
#include "C_Warrior.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/C_FeetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimC_Warrior::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr)
		return;

	FRotator rotator(0, Owner->GetControlRotation().Yaw, 0);

	Direction = CalculateDirection(Owner->GetVelocity().GetSafeNormal2D(), rotator);

	bDash = Cast<AC_Warrior>(Owner)->GetIsDash();

    AC_Warrior* Warrior = Cast<AC_Warrior>(Owner);

    if (Warrior)
    {
        
        UCameraComponent* Camera = Warrior->GetCameraComponent(); 
        if (Camera)
        {
             CameraRotation = Camera->GetComponentRotation();
             FRotator CharacterRotation = Warrior->GetActorRotation();

             const float MinPitch = -35.0f; 
             const float MaxPitch = 35.0f; 

             const float MinYaw = -90.0f;
             const float MaxYaw = 90.0f;

             CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch, MinPitch, MaxPitch);
             //CameraRotation.Yaw = FMath::Clamp(CameraRotation.Yaw, MinYaw, MaxYaw);
             
            // CameraRotation.Yaw = UKismetMathLibrary::NormalizedDeltaRotator(CameraRotation, CharacterRotation).Yaw;

             
             if (!Warrior->GetIsStun())
             {
              
                 FRotator WorldRotation = Camera->GetComponentRotation();  
                 FRotator CharacterWorldRotation = Warrior->GetActorRotation(); 

                 FRotator CameraLocalRotation = UKismetMathLibrary::NormalizedDeltaRotator(WorldRotation, CharacterWorldRotation);

                 if (CameraLocalRotation.Yaw > -90.0f && CameraLocalRotation.Yaw < 90.0f)
                 {
                     CameraRotation.Yaw = FMath::Clamp(CameraLocalRotation.Yaw, MinYaw, MaxYaw);
                 }
                 else
                 {
                     CameraRotation.Yaw = UKismetMathLibrary::NormalizedDeltaRotator(CharacterWorldRotation, CharacterWorldRotation).Yaw;
                 }
              
             }
        

           
        }
    }

    UC_FeetComponent* feetComponent = Cast<UC_FeetComponent>(Owner->GetComponentByClass(UC_FeetComponent::StaticClass()));

    if (feetComponent)
    {

        FeetData = feetComponent->GetData();
    }

}
