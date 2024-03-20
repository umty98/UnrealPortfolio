// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/C_Weapon.h"
#include "C_Sword.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_Sword : public AC_Weapon
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void Attack() override;

	virtual void BeginCombo() override;

	void SetCanCombo(bool CanCombo) { this->bCanCombo = CanCombo; }

	void ActivateCollision();
	void DeactivateCollision();

	void QSkillComboFinish();
	void QSkillComboStart();

	void ESkillComboStart();
	void ESkillComboFinish();

	class UAnimMontage* GetQSkillComboFinishMontage() const;
	class UAnimMontage* GetESkillStartMontage() const;

private:
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
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bCanCombo = false;

	UPROPERTY(BlueprintReadOnly)
	bool bBeginCombo = false;

	UINT ComboCount = 0;

	class UShapeComponent* BodyCollider;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USoundBase* HitSound;
};
