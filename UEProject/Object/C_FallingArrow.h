// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "C_FallingArrow.generated.h"

UCLASS()
class UEPROJECT_API AC_FallingArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_FallingArrow();

	void SetOwnerController(AController* NewOwner) { OwnerController = NewOwner; }
	AController* GetOwnerController() const { return OwnerController; }

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;

	class UCapsuleComponent* GetCapsuleComponent() { return CapsuleComponent; }

	class UNiagaraComponent* GetNiagaraComponent() { return NiagaraComponent; }

	class UNiagaraSystem* GetNiagaraSystem() { return ArrowNiagaraSystem; }

	void ActivateCollision();
	void DeactivateCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNiagaraSystem* ArrowNiagaraSystem;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraComponent;

	UFUNCTION()
	void OnBodyColliderBeginOverlap
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

	void DropArrow();

	void SetIsFallingTrue();

protected:
	TArray<class AC_Enemy*> Victims;

private:
	AController* OwnerController;

	bool bIsFalling = false;
};
