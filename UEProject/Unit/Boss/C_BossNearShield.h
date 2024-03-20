// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_BossNearShield.generated.h"

UCLASS()
class UEPROJECT_API AC_BossNearShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_BossNearShield();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* ShieldActiveSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ShieldActiveComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* ShieldExplosiveSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ShieldExplosiveComponent;

	
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

	UPROPERTY(VisibleAnywhere, Category = "Shield")
	class USphereComponent* ShieldCollider;

	class UNiagaraComponent* GetShieldActiveComponenent() { return ShieldActiveComponent; }
	class UNiagaraComponent* GetShieldExplosiveComponenent() { return ShieldExplosiveComponent; }

	void SetOwnerController(AController* NewOwner) { OwnerController = NewOwner; }
	AController* GetOwnerController() const { return OwnerController; }

	void SetOwner(class AC_Unit* NewOwner) { Owner = NewOwner; }
	class AC_Unit* GetOwner() const { return Owner; }

	void ActivateCollision();
	void DeactivateCollision();

private:
	AController* OwnerController;

	TArray<class AC_Unit*> Victims;

	class AC_Unit* Owner;

};
