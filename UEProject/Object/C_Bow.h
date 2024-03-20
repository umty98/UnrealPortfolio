// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/C_Weapon.h"
#include "C_Bow.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_Bow : public AC_Weapon
{
	GENERATED_BODY()

public:
	virtual	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	void AppearArrow();
	void FireArrow();

	class UAnimMontage* GetBowFireMontage() const;

	void PlayFireArrow();

	virtual void BeginCombo() override;

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

	bool GetArrowAttachedInBow() { return bIsAttachedInBow; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AC_Arrow> ArrowClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USoundBase* HitSound;

private:
	void CreateArrows();

	TArray<class AC_Arrow*> Arrows;

	float AddPlusSpeed;

	bool bIsAttachedInBow = false;

};
