// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_BossFollowingSpear.generated.h"

UCLASS()
class UEPROJECT_API AC_BossFollowingSpear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_BossFollowingSpear();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* FollowingArrowSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* FollowingArrowComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* FollowingArrowSpawnSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* FollowingArrowSpawnComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UNiagaraSystem* FollowingArrowExplosiveSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* FollowingArrowExplosiveComponent;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	void FireSpear();
	void SpawnSpear();
	void DestorySpear();
	void SpearExplosive();
	void DestorySpearIfNotHit();

	class UNiagaraComponent* GetFollowingArrowComponent() { return FollowingArrowComponent; }

	void SetOwnerController(AController* NewOwner) { OwnerController = NewOwner; }
	AController* GetOwnerController() const { return OwnerController; }

	void SetOwner(AC_Unit* NewOwner) { Owner = NewOwner; }
	AC_Unit* GetOwner() const { return Owner; }

	//void ActivateNiagara();

	void ActivateCollision();
	void DeactivateCollision();

private:
	AController* OwnerController;

	TArray<class AC_Unit*> Victims;

	FTimerHandle SpawnTimerHandle;
	FTimerHandle DestructionTimerHandle;

	class AC_Unit* Owner;

	bool bIsFired = false;
	bool bCanExplode = false;
};
