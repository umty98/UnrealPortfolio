// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Arrow.generated.h"

UCLASS()
class UEPROJECT_API AC_Arrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Arrow();

	bool GetIsActive() { return bIsActive; }

	bool GetIsAttached() { return bIsAttached; }
	bool GetIsFired() { return bIsFired; }

	void SetOwner(class AC_Warrior* warrior)
	{
		Owner = warrior;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnColliderBeginOverlap
	(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComponent,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& sweepResult
	);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetActive(bool IsActive);

	void SetDirection(FVector Direction);

	void AppearArrow();

	void Fire(float PlusSpeed);

	void DeactiveArrow();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Speed = 2000.0f;

	UPROPERTY(VisibleAnywhere)
	bool bIsAttached = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsFired = false;

	UPROPERTY(VisibleAnywhere)
	float RightMousePressedTime = 0.0f;

private:
	void DeactivateArrowIfNotHit();

private:
	bool bIsActive = false;

	FTimerHandle FireTimerHandle;

	class UShapeComponent* Collider;

private:
	class AC_Warrior* Owner;
};
