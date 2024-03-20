// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_PetPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEPROJECT_API UC_PetPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UC_PetPatrolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);

	bool& GetHasRandomLocation() { return bHasValidRandomLocation; }

	void InitializeComponent(class AC_Warrior* InWarrior);

	class AC_Warrior* FindWarrior();
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float AcceptanceRadius = 500.0f;

	FVector LastRandomLocation;
	bool bHasValidRandomLocation = false;

	class AC_Warrior* warrior = nullptr;
};
