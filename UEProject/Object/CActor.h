// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActor.generated.h"

UCLASS()
class UEPROJECT_API ACActor : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* cube;
	
public:	
	// Sets default values for this actor's properties
	ACActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
