// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit/C_Unit.h"
#include "C_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API AC_Enemy : public AC_Unit
{
	GENERATED_BODY()
public:
	AC_Enemy();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
public:
	void LaunchIntoAirQSKill();
	void KnockBack(FVector Direction);
	void EnemyBounce();
	void ApplyForce(FVector Force);

	void LaunchIntoAirESkill();

	void LookAround();

	bool& GetIsFloating() { return bIsFloating; }
	bool& GetIsHitByQSkill() { return bIsHitByQSkill; }
	bool& GetIsLeader() { return bIsLeader; }

	void Landed(const FHitResult& Hit);

	class UBehaviorTree* GetBT() { return BehaviorTree; }

	class UC_PatrolComponent* GetPatrolComponent() { return PatrolComponent; }

	//
	void SetOwnerController(AController* NewOwner) { OwnerController = NewOwner; }

	AController* GetOwnerController() const { return OwnerController; }

	void SetLeader(AC_Enemy* NewLeader);

	UPROPERTY(VisibleAnywhere)
	AC_Enemy* LeaderEnemy;

	FVector RelativePositionToLeader;

	void PlayReactMontage();

	FVector GetLeaderStartLocation() { return LeaderStartLocation; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UC_PatrolComponent* PatrolComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QSkill")
	class UParticleSystem* QSKillLand;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UAnimMontage* LookAroundMontage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UAnimMontage* ReactMontage;

	UPROPERTY(VisibleAnywhere)
	bool bIsLeader = false;

private:
	bool bIsHitByQSkill = false;
	bool bIsHitByESkill = false;

	class UCapsuleComponent* CapsuleComp;

	AController* OwnerController;

	FVector LeaderStartLocation;


};
