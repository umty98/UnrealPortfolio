// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_BossFallingSpear.generated.h"

UCLASS()
class UEPROJECT_API AC_BossFallingSpear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_BossFallingSpear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* FallingSpearSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* FallingSpearComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDecalComponent* DecalComponent;

	UFUNCTION()
	void OnSpearColliderBeginOverlap
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

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;

	void SetOwnerController(AController* NewOwner) { OwnerController = NewOwner; }
	AController* GetOwnerController() const { return OwnerController; }

	void SetOwner(AC_Unit* NewOwner) { Owner = NewOwner; }
	AC_Unit* GetOwner() const { return Owner; }

	void ActivateNiagara();

	void ActivateCollision();
	void DeactivateCollision();

private:
	AController* OwnerController;

	TArray<class AC_Unit*> Victims;

	class AC_Unit* Owner;
};
