// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "C_ESkillColliderBox.generated.h"

UCLASS()
class UEPROJECT_API AC_ESkillColliderBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_ESkillColliderBox();

	void SetOwnerController(AController* NewOwner) { OwnerController = NewOwner; }
	AController* GetOwnerController() const { return OwnerController; }
	void SetOwner(AC_Unit* NewOwner) { Owner = NewOwner; }
	AC_Unit* GetOwner() const { return Owner; }


	const TArray<class AC_Unit*>& GetVictims() const { return Victims; }

	bool& GetCanLaunchByESkill() { return bCanLaunchByESkill; }

//	FVector GetScaledBoxExtent() const { return BoxCollider ? BoxCollider->GetScaledBoxExtent() : FVector::ZeroVector; }
	//FVector GetColliderCenter() const { return BoxCollider ? BoxCollider->GetComponentLocation() : FVector::ZeroVector; }

public:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;

	void ActivateCollision();
	void DeacivateCollision();

	UFUNCTION()
	void OnESkillOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	TArray<class AC_Unit*> Victims;

private:
	AController* OwnerController;

	bool bCanLaunchByESkill = false;
	float ESkillDamage;

	class AC_Unit* Owner;
};
