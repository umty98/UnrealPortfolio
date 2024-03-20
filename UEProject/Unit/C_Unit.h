// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"
#include "C_Unit.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	UNARMED,
	SWORD,
	RIFLE,
	BOW,
	ORB
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	NORMAL,
	STUNNED,
	BLOCK
};

UCLASS()
class UEPROJECT_API AC_Unit : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Unit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EWeaponState GetWeaponType() { return WeaponType; }
	ECharacterState GetCharacterType() { return CharacterState; }


	void StrafeOn();
	void StrafeOff();

	void UnEquipWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();

	virtual void  OnAttack();
	virtual void OffAttack();

	UFUNCTION(BlueprintCallable)
	void  EnableAttack();

	UFUNCTION(BlueprintCallable)
	void DisableAttack();

	void  EnableCombo();
	void DisableCombo();

	void BeginCombo();

	void SetCanMove(bool CanMove) { this->bCanMove = CanMove; }

	class AC_Weapon* GetCurrentWeapon()
	{
		if (Weapons.Contains(WeaponType))
			return Weapons[WeaponType];

		return nullptr;
	}

	void ChangeWeapon(EWeaponState weaponState);

	bool GetIsAttacking() { return bIsAttacking; }

	void Landed(const FHitResult& Hit);

	bool CanBeHit() {return bCanBeHit; }

	float GetHp() { return CurHp; }

	float GetMaxHp() { return MaxHp; }

	bool GetIsDead() { return bIsDead; }

	bool& GetIsHit() { return bIsHit; }

	FVector GetPlayerLocation() { return PlayerLocation; }

	virtual FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(TeamID); }

	bool& GetCanMove() { return bCanMove; }

	//Stun
	void ApplyStun(float Duration);
	void ReduceStunTime();
	void ResetStun();
	bool GetIsStun() { return bIsStunned; }
	bool GetCanStandUp() { return bCanStandUp; }
	void CharacterStateNormal();
	void KnuckBack();
	float& GetStunDuration() { return StunDuration; }

	//Block
	void StartBlocking();
	void EndBlocking();
	bool GetIsBlocking() { return bIsBlocking; }

	//AttackDelay
	bool& GetAttackDelay() { return bIsAttackDelay; }

	void ResetAttackDelay();
	void SetAttackDelayFalse();

private:
	float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION()
	void Terminate();

	UFUNCTION(BlueprintCallable)
	void RestoreColor();

	UFUNCTION()
	void ResetHit();

protected:
	UPROPERTY(EditAnywhere)
	float  RotSpeed = 50.0f;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<EWeaponState, TSubclassOf<class AC_Weapon>> WeaponClasses;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<EWeaponState, AC_Weapon*> Weapons;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EWeaponState WeaponType = EWeaponState::UNARMED;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ECharacterState CharacterState = ECharacterState::NORMAL;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EWeaponState NextEquipWeapon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UAnimMontage* HitMontage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UWidgetComponent* NameWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UWidgetComponent* HpWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USoundBase* EquipSwordSound;

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<ETeamAttitude::Type> TeamID = ETeamAttitude::Neutral;

protected:

	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere)
	bool bCanMove = true;

	bool bIsLoop = false;

	//bool bPendingDeath = false;

	bool bCanBeHit = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxHp = 50.0f;

	UPROPERTY(VisibleAnywhere)
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsFloating = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsHit = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CurHp = MaxHp;

	FVector direction;

	class UMaterialInstanceDynamic* BodyMaterial;

	FLinearColor BodyColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bTestActorCollision = false;

	FVector PlayerLocation; 

	FTimerHandle StunTimerHandle;

	UPROPERTY(VisibleAnywhere)
	bool bIsStunned = false;
	UPROPERTY(VisibleAnywhere)
	float StunDuration;

	float StunStartTime;

	bool bIsBlocking = false;

	bool bCanStandUp = false;

	FTimerHandle TimerHandle_EnemyAttackDelay;

	UPROPERTY(VisibleAnywhere)
	bool bIsAttackDelay = false;

	
};
