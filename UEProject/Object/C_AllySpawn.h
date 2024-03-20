// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_AllySpawn.generated.h"

UCLASS()
class UEPROJECT_API AC_AllySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_AllySpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDecalComponent* DecalComponent;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ColliderBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AC_AllyWarrior> AllyClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn")
	class UParticleSystem* SpawnParticleSystem;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spawn")
	class UParticleSystem* SpawnLocationSystem;

	void CheckAndPlayParticleSystem();
	void UpdateParticleSystem(bool bWarriorNear);

private:
	UFUNCTION()
	void OnComponentColliderBeginOverlap
	(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComponent,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& sweepResult
	);

private:
	void SpawnAlly();
	void SpawnNextAlly();
	void DestroySpawnActor();

	TArray<class AC_AllyWarrior*> Allies;

	FRotator PlayerRotation;
	FTimerHandle SpawnTimerHandle;
	int32 CurrentSpawnIndex = 0;
	TArray<FVector> SpawnPositions;

	class AC_Warrior* Warrior;


	UParticleSystemComponent* SpawnParticleComponent;
	UParticleSystemComponent* SpawnLocationComponent;

	UPROPERTY(VisibleAnywhere)
	bool bIsWarriorNear = false;
};
