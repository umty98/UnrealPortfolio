// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit/C_Unit.h"
#include "C_Boss.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FBossSkill
{
	GENERATED_BODY()

	bool bCanUseSkill = true;

	bool bSKillUsed = false;

	FTimerHandle BossSkillTimerHandle;
};

UCLASS()
class UEPROJECT_API AC_Boss : public AC_Unit
{
	GENERATED_BODY()
public:
	AC_Boss();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	class UBehaviorTree* GetBT() { return BehaviorTree; }

	bool& GetIsCenter() { return bIsCenter; }

	bool GetCanUseSkill() { return bCanUseSkill; }
	
	//Shield
	void ShieldActive();
	void DestroyShield();
	void ShieldExplosive();
	void ActivateShieldCollision();
	void DeactivateShieldCollision();
	void SetUseShieldSkill();
	bool GetCanUseShieldSkill() { return shieldSkill.bCanUseSkill; }
	bool GetIsShieldSkillUsed() { return shieldSkill.bSKillUsed; }
	//Middle
	void MiddleAttackActive();
	void DestroyMiddleAttack();
	void ActivateMiddleCollision();
	void DeactivateMiddleCollision();
	void SetUseMiddleSkill();
	bool GetCanUseMiddleSkill() { return middleSkill.bCanUseSkill; }
	bool GetIsMiddleSkillUsed() { return middleSkill.bSKillUsed; }
	//FallingSpear
	void FallingSpearActive();
	void FallingSpearSpawn();
	void ActivateSpearCollision();
	void DeactivateSpearCollision();
	void DestroyFallingSpear();
	void SetUseSpearSkillTrue();
	bool GetCanUseSpearSkill() { return fallingSpearSkill.bCanUseSkill; }
	bool GetIsFallingSpearSkillUsed() { return fallingSpearSkill.bSKillUsed; }
	//FollowingSpear
	void FollowingSpearActive();
	void SetUseFollowingSpearTrue();
	bool GetCanUseFollowingSkill() { return followingSpearSkill.bCanUseSkill; }
	bool GetIsFollowingSkillUsed() { return followingSpearSkill.bSKillUsed; }
	void SetIsFollowingSkillUsedTrue();
	bool GetCanUseFarSkill();

	void CanUseSkillTrue();
	void SetWarriorLocation(FVector warriorLocation) { WarriorLocation = warriorLocation; }

	bool GetCanActive() { return bCanActive; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere)
	bool bIsCenter = false;

	UPROPERTY(VisibleAnywhere)
	bool bCanUseSkill = true;

	bool bIsSkillUsed = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AC_BossNearShield> ShieldClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AC_BossMiddleAttack> MiddleClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AC_BossFallingSpear> FallingSpearClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AC_BossFollowingSpear> FollowingSpearClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SideWalk")
	class UAnimMontage* LeftWalkMontage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SideWalk")
	class UAnimMontage* RightWalkMontage;

private:
	class AC_BossNearShield* shield;
	class AC_BossMiddleAttack* middle;
	class AC_BossFallingSpear* fallingSpear;
	class AC_BossFollowingSpear* followingSpear;

	FVector WarriorLocation;

	FBossSkill shieldSkill;
	FBossSkill middleSkill;
	FBossSkill fallingSpearSkill;
	FBossSkill followingSpearSkill;

	//bool bCanUseSkill = true;

	class AC_Hud* GetHud() const;
	AC_Hud* CachedHud;

	FVector StartLocation;
	bool bMoveLeft = true;
	bool bIsSCenter;
	bool ReturningToStart = false;
	float TotalMovedDistance;
	const float MoveDistance = 300.0f;
	const float CloseEnough = 50.0f;

	bool bCanActive = false;
};
