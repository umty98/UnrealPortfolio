// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "C_Weapon.generated.h"

USTRUCT(BlueprintType)
struct FComboData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Animation;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	bool bCanMove;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitNiagara;
};

UCLASS()
class UEPROJECT_API AC_Weapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack();
	virtual void EndAttack();

	virtual void BeginCombo();

	void   AttachWeapon();
	void DetachWeapon();

	void AttachWeapon(class USceneComponent* Parent);

	FName GetWeaponName() { return WeaponName; }

	class UAnimMontage* GetEquipMontage() { return EquipMontage; }
	class UAnimMontage* GetUnEquipMontage() { return UnEquipMontage; }

protected:
	//UPROPERTY(BlueprintReadOnly, EditAnywhere)
	//class USkeletalMeshComponent* mesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UDataTable* SwordComboDataTable;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* UnEquipMontage;

	UPROPERTY(EditDefaultsOnly)
	FName EquipSocketName;

	UPROPERTY(EditDefaultsOnly)
	FName UnEquipSocketName;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponName;

	TArray<FComboData*> SwordComboData;

	class AC_Unit* Owner;
	class AC_Warrior* warrior;

	TArray<AC_Unit*> Victims;
};
