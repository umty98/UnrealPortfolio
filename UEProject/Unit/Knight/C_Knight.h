// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../C_Unit.h"
#include "C_Knight.generated.h"


UCLASS()
class UEPROJECT_API AC_Knight : public AC_Unit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Knight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);

	void OnRotateHorizontal(float Axis);
	void OnRotateVertical(float Axis);

	void OnJump();
	void OnSwordKey();
	void OnRifleKey();

	bool GetIsFlip() { return bIsFlip; }

protected:
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera;

private:
	bool bIsFlip = false;
};
