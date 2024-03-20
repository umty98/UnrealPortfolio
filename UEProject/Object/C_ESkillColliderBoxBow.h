// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "C_ESkillColliderBoxBow.generated.h"

UCLASS()
class UEPROJECT_API AC_ESkillColliderBoxBow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_ESkillColliderBoxBow();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
