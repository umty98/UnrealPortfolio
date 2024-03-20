// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_PatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEPROJECT_API UC_PatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_PatrolComponent();

protected:
	virtual void BeginPlay() override;

public:	
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);

	void UpdateNextIndex();

	bool& GetHasRandomLocation() { return bHasValidRandomLocation; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class AC_PatrolPath* Path;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Index;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsReverse;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float AcceptanceRadius = 75.0f;

	FVector LastRandomLocation;
	bool bHasValidRandomLocation = false;
};
		

