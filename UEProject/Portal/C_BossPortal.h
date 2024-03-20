// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_BossPortal.generated.h"

UCLASS()
class UEPROJECT_API AC_BossPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_BossPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBoxComponent* ColliderBox;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USphereComponent* SphereComponent;

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

	void CheckForEnemiesAndActivatePortal();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "portal")
	class UParticleSystem* PortalActiveSystem;

	UParticleSystemComponent* PortalActiveComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "portal")
	class UNiagaraSystem* PortalDefenceSystem;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* PortalDefenceComponent;
	
public:	
	// Called every frame

private:
	float AccumulatedTime = 0.0f;
	float CheckInterval = 2.0f;

	bool bIsPortalCanActive = false;
};
