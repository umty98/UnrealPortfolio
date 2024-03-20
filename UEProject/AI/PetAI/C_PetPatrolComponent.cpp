// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PetAI/C_PetPatrolComponent.h"
#include "Unit/MousePet/C_MousePet.h"
#include "Unit/Warrior/C_Warrior.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavigationSystem.h"

// Sets default values for this component's properties
UC_PetPatrolComponent::UC_PetPatrolComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UC_PetPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	AC_MousePet* pet = Cast<AC_MousePet>(GetOwner());
	AC_Warrior* Warrior = FindWarrior();

	/*
	if (Warrior)
	{
		FString Message = FString::Printf(TEXT("Patrol: %s"), *Warrior->GetName());
		UKismetSystemLibrary::PrintString(GetWorld(), Message, true, false, FLinearColor::White, 5.0f);
	}
	*/

	if (pet)
	{
		InitializeComponent(Warrior);
	}
	
}

bool UC_PetPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius)
{
	OutLocation = FVector::ZeroVector;

	OutAcceptanceRadius = AcceptanceRadius;

	AC_MousePet* pet = Cast<AC_MousePet>(GetOwner());

	
	if (!bHasValidRandomLocation)
	{
		if (warrior)
		{
			LastRandomLocation = warrior->GetRandomPoint(1000.0f);
			bHasValidRandomLocation = true;

			/*
			FString LocationString = LastRandomLocation.ToString();
			FString Message = FString::Printf(TEXT("Destination Location: %s"), *LocationString);

			UKismetSystemLibrary::PrintString(GetWorld(), Message);
			*/
		}
	}
	
	OutLocation = LastRandomLocation;
	return bHasValidRandomLocation;



	return false;
}

void UC_PetPatrolComponent::InitializeComponent(AC_Warrior* InWarrior)
{
	warrior = InWarrior;
}

AC_Warrior* UC_PetPatrolComponent::FindWarrior()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			AC_Warrior* Warrior = Cast<AC_Warrior>(PC->GetPawn());
			if (Warrior)
			{
				return Warrior;
			}
		}
	}
	return nullptr;
}

