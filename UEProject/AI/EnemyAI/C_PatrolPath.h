// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_PatrolPath.generated.h"

UCLASS()
class UEPROJECT_API AC_PatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_PatrolPath();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	class USplineComponent* GetSpline() { return Spline; }

	bool GetIsLoop() { return bIsLoop; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USplineComponent* Spline;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsLoop = true;
};
