// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Unit/C_Unit.h"
#include "Unit/Enemy/C_Enemy.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "C_Warrior.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FSkillCooldown
{
	GENERATED_BODY()

	bool bIsSkillActive = false;

	bool bIsTimerActive = false;

	FTimerHandle CooldownTimerHandler;

	float CooldownDuration;

	float CooldownStartTime;
};

UCLASS()
class UEPROJECT_API AC_Warrior : public AC_Unit
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AC_Warrior();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnAttack() override;
	virtual void OffAttack() override;

	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);

	void OnRotateHorizontal(float Axis);
	void OnRotateVertical(float Axis);
	void OnMouseWheelScroll(float AxisValue);

	void StartRunning();
	void StopRunning();

	void OnJump();
	void JumpOrDash();
	void Dash();
	void Landed(const FHitResult& Hit);

	void SetRotationToCameraDirection();
	bool PerformRaycastFromCamera(FVector& OutHitLocation);
	void RotateTowardsRaycastHit();
	void RotateTowardsRaycastHitForZoom();
	class UCameraComponent* GetCameraComponent() const { return Camera; }
	bool GetIsMoving() { return bIsMoving; }

	void StopGhostTrail();

	void OnSwordKey();
	void OnRifleKey();
	void OnBowKey();

	bool GetIsFlip() { return bIsFlip; }
	bool GetIsDash() { return bIsDashing; }

	//Target
	void AutoAim();
	void AutoAimAtClosetEnemy();
	bool IsTargetLocked() const;
	AC_Unit* GetTargetedEnemy() const;
	void SetTargetedEnemy(AC_Unit* NewTarget);
	
	void StopNiagaraEffect(UNiagaraComponent* NiagaraComponent);

	void ActivateGhostTrail();
	void DeactivateGhostTrail();

	void ActivateParticle();
	void DeactiveParticle();

	//Skills
	void TotalQSkill();
	void TotalESkill();
	//QSkill_Sword
	void QSkillUp();
	void SetQSkillCollider();
	void ActivateQSkillCollider();
	void DeactiveQSkillCollider();
	//QSKillComboFinished_Sword
	void QSkillComboFinish();
	void SetTargetEnemyAir(AC_Enemy* NewTarget);
	void LaunchWarriorToAir();
	void WarriorBounce();
	void MakeBounceInAir();
	void WarriorApplyForce();
	void AfterQComboFinish();
	void SetQComboFinishMontageSpeed(float speed);
	void PrintCurrentMontagePlayRate();
	void QSKillComboStart();
	//ESkill_Sword
	void ESkillStart();
	UFUNCTION()
	void ESkillDash();
	UFUNCTION()
	void ESkillCombo();
	UFUNCTION()
	void DisableColliderCollision();
	void LaunchAfterESkill();
	UFUNCTION()
	void LandAfterESkill();
	UFUNCTION()
	void ESkillLastGhostTrail();
	void ESkillFinishEffect();
	void ESKillSideThunder();
	void SpawnRandomThunder();
	void ActivateSkillCamera();
	void DeactivateSkillCamera();
	//ESKill_Sword_ColliderBox
	void SpawnESkillColliderBox();
	UFUNCTION()
	void DeactivateESkillLandEffect();
	UFUNCTION()
	void DeactivateESkillAirEffect();
	UFUNCTION()
	void DestroyESkillColliderBox();
	FVector CalculateCenterOfEnemies(const TArray<AC_Enemy*>& Enemies);
	//QSkill_Bow
	void JumpBack();
	void ShootArrowAfterJumpBack();

	//ESkill_Bow
	void SetBowESkillArea();
	void SpawnArrows();
	void DestroyESkillArea();
	UFUNCTION()
	void DeactivateESkillBowLandEffect();

	//Kick
	void Kick();
	void SetKickCollider();
	void ActivateKickCollider();
	void DeactivateKickCollider();
	UFUNCTION()
	void ResetKick();

	//Bow
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void ActivateChargingEffect();
	void PauseBowAttackMontage();
	void ResumeBowAttackMontage();
	void AppearArrow();
	void FireArrow();
	float CalculateAdditionalSpeed(float HoldTime);
	float GetBowAdditionalSpeed() { return AdditionalSpeed; }
	void ShowTrajectory();
	void ZoomIn();
	void ZoomOut();
	bool GetIsBowCameraActive() { return bIsBowCameraActive; }
	float GetVerticalRotationInput() { return VerticalRotationInput; }

	//Utility
	void BlockOn();
	void BlockOff();


	//Test
	void TestSample();

	//CameraShake
	void StunnedCameraShake();
	void QSkillCameraShake();
	void SucceedBlockCameraShake();

	//HUD(Mp,Exp)
	void UpdateWeaponIcons();
	void RecoverMP(int32 Amount);
	void AddExp(int32 Amount);
	void LevelUp();
	void ActiveLevelUpParticle();


	//HUD(Skill)
	//Sword-Q
	void StartQSSkillTimer(float cooltimer);
	UFUNCTION()
	void UpdateQSCooldown();
	UFUNCTION()
	void ResetSwordQSkill();
	//Sword-E
	void StartESSkillTimer(float cooltimer);
	UFUNCTION()
	void UpdateESCooldown();
	UFUNCTION()
	void ResetSwordESkill();
	//Bow-Q
	void StartQBSkillTimer(float cooltimer);
	UFUNCTION()
	void UpdateQBCooldown();
	UFUNCTION()
	void ResetBowQSkill();
	//Bow-E
	void StartEBSkillTimer(float cooltimer);
	UFUNCTION()
	void UpdateEBCooldown();
	UFUNCTION()
	void ResetBowESkill();
	//ResetAll
	void ResetAllSkill();

	//Freeze&stun
	void Freezed();
	void ResetFreezed();
	void StunnedBarOn();
	void UpdateStunnedBar();

	//pet
	FVector GetRandomPoint(float Radius) const;
	void PetKey();
	void SpawnPet();
	UFUNCTION()
	void DestroyPetSpawnArea();

	//Map
	void MapChange();
	void BossMapSetting();
	void SavePlayerInstance();
	void GetPlayerInstance();

	//Trail
	void ActivateTrailEffect();
	void DeactivateTrailEffect();

	//SpawnAlly
	void ActivateSpawnCamera();
	void DeactivateSpawnCamera();

	UFUNCTION(BlueprintCallable)
	void HideAll();

	UFUNCTION(BlueprintCallable)
	void UnHideAll();


private:
	UFUNCTION()
	void OnQSkillOverlap
	(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnKickOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* BowSpringArm;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* BowCamera;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* MiniMapSpringArm;

	UPROPERTY(EditDefaultsOnly)
	class USceneCaptureComponent2D* MiniMapCaptureComponent;

	UPROPERTY(EditDefaultsOnly)
	class AC_SkillCamera* SkillCamera;

	//SwordTrail
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNiagaraSystem* SwordTrail;
	UNiagaraComponent* SwordTrailComponent;


	//Run
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNiagaraSystem* Run_Niagara;

	//Dash
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNiagaraSystem* Dash_Niagara;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNiagaraSystem* Dash_GhostTrail_Niagara;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class USoundBase* DashSound;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UParticleSystem* DashEndEffect;

	//QSkill_Sword
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QSkill_Sword")
	class UAnimMontage* QAttackSkillMontage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QSkill_Sword")
	UNiagaraSystem* QAttackSkill_Niagara;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QSkill_Sword")
	class USoundBase* QSkillSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QSkill_Sword")
	class UParticleSystem* QSkillEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QSkill_Sword")
	UBoxComponent* QSkillCollider;
	//QCombo_Sword
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QSkill_Sword")
	class UAnimMontage* QComboSkillMontage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "QSkill_Sword")
	UNiagaraSystem* QSkillLaunch_Niagara;
	//Kick
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Kick")
	class UAnimMontage* KickMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kick")
	UBoxComponent* KickCollider;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Kick")
	class USoundBase* KickSound;
	//ESkill_Sword
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	UNiagaraSystem* ESkillDash_Niagara;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	UNiagaraSystem* ESkill_GhostTrail_Niagara;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ESkill_Sword")
	class UAnimMontage* ESKillLandMontage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	class UParticleSystem* ESkillFloor;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	class UParticleSystem* ESkillAir;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	class UParticleSystem* ESkillCenterThunder;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	class UParticleSystem* ESkillSideThunder;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	class USoundBase* MainThunderSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	class USoundBase* SideThunderSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Sword")
	TSubclassOf<class AC_ESkillColliderBox> BPESkillColliderBox;

	//Bow
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	class USoundBase* BowStartSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	class USoundBase* BowReleaseSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	class USoundBase* BowFireSound;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	UNiagaraSystem* TrajectoryNiagara;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	class UParticleSystem* BowChargingParticle;



	//QSkill_Bow
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "QSkill_Bow")
	class UAnimMontage* QJumpBackMontage;

	//ESkill_Bow
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Bow")
	TSubclassOf<class AC_FallingArrow> FallingArrow;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ESkill_Bow")
	class UParticleSystem* ESkillBowFloor;

	//Freeze
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Freeze")
	UNiagaraSystem* FreezeNiagara;

	//CameraShake
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> QSkillCameraShakeClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> SucceedBlockCameraShakeClass;


	UPROPERTY(VisibleAnywhere)
	float LeftMouseHoldTime = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float AdditionalSpeed = 0.0f;

	//Widget
	//UPROPERTY()
	//WC_PlayerUI* PlayerUI;

	// 위젯 클래스를 저장하는 변수
	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<WC_PlayerUI> PlayerUIClass;

	//Mp
	float MaxMp = 100.0f;
	float GetMp() { return CurMp; }
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CurMp = MaxMp;
	//Exmp
	float CurExp = 0.0f;
	float GetExp() { return CurExp; }
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxExp = 100.0f;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LevelUp")
	class UParticleSystem* LevelUpParticle;


	//pet
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pet")
	TSubclassOf<class AC_MousePet> mousePet;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pet")
	TSubclassOf<class AC_PetSpawn> PetSpawnArea;

private:
	bool bIsBowCameraActive = false;

	bool bIsFlip = false;
	bool bIsDash = false;

	FVector LastInputDirection;
	bool bHasDashed = false;
	bool bIsDashInput;
	bool bIsDashing = false;
	bool bIsKicking = false;

	bool bIsRunning = false;

	bool bIsCharging = false;

	bool bIsAutoAiming = false;

	bool bIsQSkillActivated = false;
	bool bIsWarriorLaunched = false;
	bool bIsBowESkillAcitvated = false;

	uint32 ESkillDashCount = 0;
	FRotator PreDashRotation;

	FTimerHandle TimerHandle;
	FTimerHandle ArrowTimerHandle;
	FTimerHandle QSkillResetTimer;
	FTimerHandle FreezeTimerHandle;
	FTimerHandle StunnedTimerHandle;
	FTimerHandle DashTimerHandle;

	uint32 ThunderSpawnCount = 0;
	FVector BoxExtent;
	FVector BoxCenter;

	//Run&&Dash
	UNiagaraComponent* RunNiagaraComponent;
	UNiagaraComponent* DashNiagaraComponent;
	UNiagaraComponent* DashGhostNiagaraComponent;
	//SwordQSkill
	UNiagaraComponent* QSkillComponent;
	UNiagaraComponent* GhostRunComponent;
	//SwordESkill
	UNiagaraComponent* ESkillGhostNiagaraComponent;
	UNiagaraComponent* ESkillDashComponent;
	UNiagaraComponent* QSkillLaunchComponent;
	//Freeze
	UNiagaraComponent* FreezeComponent;

	UParticleSystemComponent* QSkillParticleComponent;
	UParticleSystemComponent* ESkillFloorComponent;
	UParticleSystemComponent* ESkillFloorBowComponent;
	UParticleSystemComponent* ESkillAirComponent;
	UParticleSystemComponent* BowChargingComponent;

	TArray<AC_Unit*> Victims;
	TArray<AC_Enemy*> TargetEnemies;
	AC_Unit* TargetedEnemy;

	class AC_ESkillColliderBox* ESkillColliderBox;
	class AC_ESkillColliderBoxBow* BowESkillArea;
	FVector BowBoxExtent;
	FVector BowBoxCenter;
	int32 ArrowSpawnCount;

	UPROPERTY(VisibleAnywhere)
	bool bIsLeftMouseDown = false;

	bool bIsBowMontageStopped = false;

	float LastArrowFallTime;
	TArray<AC_FallingArrow*> SpawnedArrows;
	UNiagaraComponent* TrajectoryComponent;

	float VerticalRotationInput;
	
	//HUD	
	class AC_Hud* GetHud() const;
	AC_Hud* CachedHud;

	int32 level = 1;

	FSkillCooldown SwordQCooldown;
	FSkillCooldown SwordECooldown;
	FSkillCooldown BowQCooldown;
	FSkillCooldown BowECooldown;

	AC_PetSpawn* PetSpawnAreaInstance = nullptr;
	bool bIsPetSpawnDecalActive = false;
	//bool bIsPetSpawned = false;
	bool bCanPetSpawnAreaMove = false;

	UPROPERTY(VisibleAnywhere)
	bool bWorldMapOn = false;
	bool bMiniMapOn = true;
	bool bIsDraggingMap = false;

	FVector MiniMapCameraLocation;

	UPROPERTY(VisibleAnywhere)
	bool bIsFreezed = false;

	float BarStunStartTime;
	bool bIsMoving;
};