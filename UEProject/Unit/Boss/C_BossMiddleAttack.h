// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_BossMiddleAttack.generated.h"

UCLASS()
class UEPROJECT_API AC_BossMiddleAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_BossMiddleAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* MiddleAttackSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* MiddleAttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDecalComponent* DecalComponent;

	UFUNCTION()
	void OnMiddleColliderBeginOverlap
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
