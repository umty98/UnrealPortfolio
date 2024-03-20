// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Warrior/C_Warrior.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/C_Sword.h"
#include "Object/C_Bow.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Unit/Enemy/C_Enemy.h"
#include "Object/C_FallingArrow.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "Object/C_ESkillColliderBox.h"
#include "Object/C_ESkillColliderBoxBow.h"
#include "Particles/ParticleSystemComponent.h" 
#include "Components/CapsuleComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Widget/C_Hud.h"
#include "Widget/WC_PlayerUI.h"
#include "AI/PetAI/C_PetAIController.h"
#include "Unit/MousePet/C_MousePet.h"
#include "Object/C_PetSpawn.h"
#include "Unit/Warrior/C_PlayerInstance.h"
#include "Camera/C_SkillCamera.h"

AC_Warrior::AC_Warrior()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(L"/Script/Engine.SkeletalMesh'/Game/Model/Warrior/Warrior.Warrior'");

	GetMesh()->SetSkeletalMesh(mesh.Object);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -87.5f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	TeamID = ETeamAttitude::Friendly;

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	BowSpringArm = CreateDefaultSubobject<USpringArmComponent>("BowSpringArm");
	BowSpringArm->SetupAttachment(RootComponent);

	BowCamera = CreateDefaultSubobject<UCameraComponent>("BowCamera");
	BowCamera->SetupAttachment(BowSpringArm);
	BowCamera->SetActive(false);

	MiniMapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapSpringArm"));
	MiniMapSpringArm->SetupAttachment(RootComponent);

	MiniMapCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCaptureComponent"));
	MiniMapCaptureComponent->SetupAttachment(MiniMapSpringArm);

	bHasDashed = false;
	
	SetQSkillCollider();
	SetKickCollider();

}

void AC_Warrior::BeginPlay()
{
	Super::BeginPlay();
	
	QSkillCollider->OnComponentBeginOverlap.AddDynamic(this, &AC_Warrior::OnQSkillOverlap);
	QSkillCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KickCollider->OnComponentBeginOverlap.AddDynamic(this, &AC_Warrior::OnKickOverlap);
	KickCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CachedHud = GetHud();

	ResetAllSkill();

	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	//UKismetSystemLibrary::PrintString(this, CurrentLevelName);

	if (CurrentLevelName == "BossMap")
	{
		if (CachedHud && CachedHud->PlayerUIInstance)
		{
			CachedHud->PlayerUIInstance->WorldMapOn(false);
			CachedHud->PlayerUIInstance->MiniMapOn(false);
			CachedHud->PlayerUIInstance->IsStunned(false);
			CachedHud->PlayerUIInstance->IsFreezed(false);
			CachedHud->PlayerUIInstance->UpdateBossHpBox(true);
		}

		GetPlayerInstance();

		//CurExp = 140.0f;
		//MaxExp = 350.0f;
		//level = 6;
		//CurHp = 120.0f;
		//MaxHp = 250.0f;
		//CurMp = 125.0f;
		//MaxMp = 150.0f;


	}
	else
	{
		if (CachedHud && CachedHud->PlayerUIInstance)
		{
			CachedHud->PlayerUIInstance->WorldMapOn(bWorldMapOn);
			CachedHud->PlayerUIInstance->MiniMapOn(bMiniMapOn);
			CachedHud->PlayerUIInstance->IsStunned(false);
			CachedHud->PlayerUIInstance->IsFreezed(false);
			CachedHud->PlayerUIInstance->UpdateBossHpBox(false);
		}

	}

	if (SwordTrail)
	{
		SwordTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(SwordTrail, GetMesh(), FName("TrailSocket"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
		if (SwordTrailComponent)
		{
			SwordTrailComponent->SetPaused(true);
			SwordTrailComponent->SetVisibility(false);
			SwordTrailComponent->SetHiddenInSceneCapture(true);
		}
	}


}

void AC_Warrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFlip)
		bIsFlip = false;

	//if (bIsDash)
		//bIsDash = false;

	if (bIsAutoAiming)
		AutoAimAtClosetEnemy();
	else
	{
		if (TargetedEnemy != nullptr)
		{
			SetTargetedEnemy(nullptr);
		}
	}

	if (bIsLoop && WeaponType == EWeaponState::BOW)
		GetCurrentWeapon()->Attack();

	if (bIsBowCameraActive)
	{
		RotateTowardsRaycastHitForZoom();
		if (bIsLeftMouseDown)
		{
			AC_Weapon* CurrentWeapon = GetCurrentWeapon();
			if (CurrentWeapon)
			{
				AC_Bow* bow = Cast<AC_Bow>(CurrentWeapon);
				if (bow->GetArrowAttachedInBow())
				{
					ShowTrajectory();
				}
			}
		}
	}

	if (bIsLeftMouseDown)
	{
		LeftMouseHoldTime += DeltaTime;
	}

	if (SpawnedArrows.Num() > 0 && GetWorld()->TimeSeconds - LastArrowFallTime > 3.0f)
	{
		for (auto& Arrow : SpawnedArrows)
		{
			if (Arrow)
			{
				Arrow->DeactivateCollision();
				Arrow->Destroy();
			}
		}
		SpawnedArrows.Empty();

		DestroyESkillArea();
	}

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateHp(GetHp(), MaxHp);
		CachedHud->PlayerUIInstance->UpdateMp(GetMp(), MaxMp);
		CachedHud->PlayerUIInstance->UpdateExp(GetExp(), MaxExp, level);
	}

	if (CurExp >= MaxExp)
		LevelUp();

	UpdateWeaponIcons();

	if (bCanPetSpawnAreaMove)
	{
		FVector HitLocation;
		if (PerformRaycastFromCamera(HitLocation))
		{
			PetSpawnAreaInstance->SetActorLocation(HitLocation);
		}
	}

	if (bIsCharging)
	{
		ActivateChargingEffect();
	}

}

void AC_Warrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AC_Warrior::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AC_Warrior::OnMoveRight);

	PlayerInputComponent->BindAxis("RotateHorizontal", this, &AC_Warrior::OnRotateHorizontal);
	PlayerInputComponent->BindAxis("RotateVertical", this, &AC_Warrior::OnRotateVertical);

	PlayerInputComponent->BindAxis("WorldMapZoom", this, &AC_Warrior::OnMouseWheelScroll);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AC_Warrior::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AC_Warrior::StopRunning);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AC_Warrior::JumpOrDash);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, this, &AC_Warrior::OnSwordKey);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, this, &AC_Warrior::OnBowKey);
	PlayerInputComponent->BindAction("Pet", EInputEvent::IE_Pressed, this, &AC_Warrior::PetKey);

	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &AC_Warrior::BlockOn);
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AC_Warrior::BlockOff);

	PlayerInputComponent->BindAction("Attack",  EInputEvent::IE_Pressed , this, &AC_Warrior::OnAttack);
	PlayerInputComponent->BindAction("Attack",  EInputEvent::IE_Released, this, &AC_Warrior::OffAttack);
	PlayerInputComponent->BindAction("AutoAim", EInputEvent::IE_Released, this, &AC_Warrior::AutoAim);

	PlayerInputComponent->BindAction("QSKill", EInputEvent::IE_Pressed, this, &AC_Warrior::TotalQSkill);
	PlayerInputComponent->BindAction("ESkill", EInputEvent::IE_Pressed, this, &AC_Warrior::TotalESkill);
	PlayerInputComponent->BindAction("Kick",   EInputEvent::IE_Pressed, this, &AC_Warrior::Kick);
	
	PlayerInputComponent->BindAction("CameraZoom", EInputEvent::IE_Pressed,  this, &AC_Warrior::ZoomIn);
	PlayerInputComponent->BindAction("CameraZoom", EInputEvent::IE_Released, this, &AC_Warrior::ZoomOut);
	PlayerInputComponent->BindAction("WorldMap", EInputEvent::IE_Pressed, this, &AC_Warrior::MapChange);
	//Test
	PlayerInputComponent->BindAction("TestKey", EInputEvent::IE_Pressed, this, &AC_Warrior::TestSample);
}

void AC_Warrior::OnAttack()
{
	if (bWorldMapOn)
	{
		bIsDraggingMap = true;	
	}
	else if (bIsPetSpawnDecalActive)
	{
		SpawnPet();
	}
	else 
	{
		if (bIsFreezed)
			return;
		
		if (bIsStunned)
		{
			//ReduceStunTime();
			StunDuration -= 0.5f;
			return;
		}


		Super::OnAttack();
	}
	

	//Super::OnAttack();

	if (WeaponType == EWeaponState::BOW)
		OnLeftMouseDown();
	
}

void AC_Warrior::OffAttack()
{
	Super::OffAttack();

	bIsDraggingMap = false;

	if (WeaponType == EWeaponState::BOW)
	{
		OnLeftMouseUp();
		ResumeBowAttackMontage();
	}
}

void AC_Warrior::OnMoveForward(float Axis)
{
	if (!bCanMove)
		return;

	if (bIsDashing)
		return;

	if (bIsStunned)
		return;

	if (bWorldMapOn)
		return;

	if (bIsFreezed)
		return;

	FVector Direction = Camera->GetForwardVector().GetSafeNormal2D();

	AddMovementInput(Camera->GetForwardVector().GetSafeNormal2D() * MoveSpeed * GetWorld()->DeltaTimeSeconds, Axis);

	if (Axis != 0.0f)
	{
		if (!bIsDashing)
			LastInputDirection += Direction * Axis;

		bIsDashInput = true;
	}
}

void AC_Warrior::OnMoveRight(float Axis)
{
	if (!bCanMove)
		return;

	if (bIsDashing)
		return;

	if (bIsStunned)
		return;

	if (bWorldMapOn)
		return;

	if (bIsFreezed)
		return;

	FVector Direction = Camera->GetRightVector().GetSafeNormal2D();

	AddMovementInput(Direction * MoveSpeed * GetWorld()->DeltaTimeSeconds, Axis);

	if (Axis != 0.0f)
	{
		if(!bIsDashing)
			LastInputDirection += Direction * Axis;

		bIsDashInput = true;
	}
}

void AC_Warrior::OnRotateHorizontal(float Axis)
{
	if (bIsDraggingMap && bWorldMapOn)
	{
		FVector CameraLocation = MiniMapCaptureComponent->GetComponentLocation();

		float DragSpeed = 100.0f;
		FVector NewLocation = CameraLocation + FVector(0.f, -Axis * DragSpeed, 0.f);

		MiniMapCaptureComponent->SetWorldLocation(NewLocation);
	}
	else
	{
		AddControllerYawInput(Axis * RotSpeed * GetWorld()->DeltaTimeSeconds);
	}
}

void AC_Warrior::OnRotateVertical(float Axis)
{
	if (bIsDraggingMap && bWorldMapOn)
	{
		FVector CameraLocation = MiniMapCaptureComponent->GetComponentLocation();

		float DragSpeed = 100.0f;
		FVector NewLocation = CameraLocation + FVector(Axis * DragSpeed, 0.f, 0.f);

		MiniMapCaptureComponent->SetWorldLocation(NewLocation);
	}
	else
	{
		AddControllerPitchInput(Axis * RotSpeed * GetWorld()->DeltaTimeSeconds);

		VerticalRotationInput = Axis;
	}
}

void AC_Warrior::OnMouseWheelScroll(float AxisValue)
{
	if (bWorldMapOn && MiniMapCaptureComponent)
	{

		float NewOrthoWidth = MiniMapCaptureComponent->OrthoWidth + (AxisValue * 100.0f);


		NewOrthoWidth = FMath::Clamp(NewOrthoWidth, 2500.0f, 6000.0f);

		MiniMapCaptureComponent->OrthoWidth = NewOrthoWidth;
	}
}

void AC_Warrior::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
	if (Run_Niagara)
	{
		RunNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			Run_Niagara,
			GetMesh(),
			FName(TEXT("None")),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		RunNiagaraComponent->SetHiddenInSceneCapture(true);
	}
}

void AC_Warrior::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	/*
	if (GhostRunComponent)
	{
		GhostRunComponent->Deactivate();
	}
	*/
	StopNiagaraEffect(RunNiagaraComponent);
}

void AC_Warrior::OnJump()
{

	Jump();

}

void AC_Warrior::JumpOrDash()
{
	if (bIsStunned)
		return;

	if (bWorldMapOn)
		return;

	if (CanJump())
	{
		Jump();
	}
	else if (!bHasDashed && GetCharacterMovement()->IsFalling())
	{

		Dash();
		bHasDashed = true;
	
	}


}

void AC_Warrior::Dash()
{
	if (bIsDashing)
		return;

	if (bIsAttacking)
		return;

	if (CurMp < 5)
		return;

	CurMp -= 5;

	bIsDashing = true;
	

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateDashCoolTime(bIsDashing);
	}
	
	/*For2DBlendSpace
	FVector DashDirection = FVector::ZeroVector;
	if (bIsDashInput && !LastInputDirection.IsNearlyZero())
	{
		DashDirection = LastInputDirection.GetSafeNormal();
	}
	else
	{
		DashDirection = GetActorForwardVector();
	}
	*/

	if (DashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DashSound, GetActorLocation());
	}

	if (Dash_Niagara)
	{
		DashNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			Dash_Niagara,
			GetActorLocation(), 
			GetActorRotation() 
		);
		DashNiagaraComponent->SetHiddenInSceneCapture(true);
	}

	if (Dash_GhostTrail_Niagara)
	{
		DashGhostNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			Dash_GhostTrail_Niagara,
			GetMesh(), 
			FName(TEXT("None")), 
			FVector::ZeroVector, 
			FRotator::ZeroRotator, 
			EAttachLocation::KeepRelativeOffset,
			true 
		);
		DashGhostNiagaraComponent->SetHiddenInSceneCapture(true);
	}
	/*
	if (GhostTrailComponent)
	{
		FVector NewScale = FVector(3.0f, 3.0f, 3.0f); 
		GhostTrailComponent->SetWorldScale3D(NewScale);
	}
	*/

	FVector DashDirection = GetActorForwardVector();
	float DashDistance = 2000.0f;
	FVector DashVelocity = DashDirection * DashDistance;

	LaunchCharacter(DashVelocity, true, true);

	bHasDashed = true;
	bIsDashInput = false;
	LastInputDirection = FVector::ZeroVector;
	//

	/*
	AActor* GhostTrailActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), GetActorLocation(), GetActorRotation());
	if (GhostTrailActor)
	{

		UNiagaraComponent* GhostTrailComponentTest= UNiagaraFunctionLibrary::SpawnSystemAttached(
			Dash_GhostTrail_Niagara,
			GhostTrailActor->GetRootComponent(),
			FName(TEXT("None")),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);


		FVector NewScale = FVector(100.0f, 100.0f, 100.0f); // 원하는 스케일 값으로 설정
		GhostTrailActor->SetActorScale3D(NewScale);
	}
	*/
}

void AC_Warrior::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bHasDashed = false;
	bIsDashing = false;
	//StopGhostTrail();
	
	
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateDashCoolTime(bIsDashing);
	}
	
	if(DashGhostNiagaraComponent)
		StopNiagaraEffect(DashGhostNiagaraComponent);
	//StopGhostTrail();

	/*
	if (DashEndEffect)
	{
		UParticleSystemComponent* DashEndEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DashEndEffect,
			this->GetTransform()
		);

		if (DashEndEffectComponent)
		{
			DashEndEffectComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f)); // NewScale은 FVector 타입이며, 원하는 스케일 값으로 설정해야 함.
		}
	}
	*/
}

void AC_Warrior::SetRotationToCameraDirection()
{
	FVector CameraForward = Camera->GetForwardVector();
	CameraForward.Z = 0; // Z 값을 0으로 설정하여 수직 방향을 무시
	CameraForward.Normalize(); // 정규화

	FRotator NewRotation = CameraForward.Rotation();
	SetActorRotation(NewRotation);
}

bool AC_Warrior::PerformRaycastFromCamera(FVector& OutHitLocation)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector Start = CameraLocation;
	FVector End = Start + (CameraRotation.Vector() * 10000); 
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); 

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		OutHitLocation = HitResult.Location;
		return true;
	}
	else
	{
		OutHitLocation = End;
		return true;;
	}

	return false;
}

void AC_Warrior::RotateTowardsRaycastHit()
{
	/*
	FVector HitLocation;
	PerformRaycastFromCamera(HitLocation);

	FVector Direction = HitLocation - GetActorLocation();
	Direction.Z = 0; 
	FRotator NewRotation = Direction.Rotation();
	SetActorRotation(NewRotation); 
	*/
	FVector HitLocation;
	PerformRaycastFromCamera(HitLocation);

	FVector Direction = HitLocation - GetActorLocation();
	Direction.Z = 0;
	FRotator TargetRotation = Direction.Rotation();
	FRotator CurrentRotation = GetActorRotation();

	float InterpSpeed = 100.0f; 
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

	SetActorRotation(NewRotation);

}

void AC_Warrior::RotateTowardsRaycastHitForZoom()
{
	FVector HitLocation;
	PerformRaycastFromCamera(HitLocation);

	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector Start = CameraLocation;
	FVector End = Start + (CameraRotation.Vector() * 10000);

	FVector MidPoint = (Start + End) / 2;

	FVector Direction = MidPoint - GetActorLocation();
	Direction.Z = 0;
	FRotator TargetRotation = Direction.Rotation();

	float InterpSpeed = 100.0f;
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, InterpSpeed);

	SetActorRotation(NewRotation);
}

void AC_Warrior::StopGhostTrail()
{
	/*
	if (GhostTrailComponent)
	{
		GhostTrailComponent->Deactivate();
	}
	*/
}

void AC_Warrior::OnSwordKey()
{
	if (bIsStunned)
		return;

	ChangeWeapon(EWeaponState::SWORD);
}

void AC_Warrior::OnRifleKey()
{
}

void AC_Warrior::OnBowKey()
{
	if (bIsStunned)
		return;

	ChangeWeapon(EWeaponState::BOW);
}

void AC_Warrior::AutoAim()
{
	bIsAutoAiming = !bIsAutoAiming;
}

void AC_Warrior::AutoAimAtClosetEnemy()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AC_Unit::StaticClass(), FoundActors);

	AC_Unit* ClosestEnemy = nullptr;
	float MinDistance = 3000.0f;

	for (AActor* otherActor : FoundActors)
	{
		AC_Unit* unit = Cast<AC_Unit>(otherActor);
		if (unit && unit != this&& unit->GetHp()>0 && unit->GetGenericTeamId() != this->TeamID)
		{
			float Distance = FVector::Dist(this->GetActorLocation(), unit->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestEnemy = unit;
			}
		}
	}
	
	if (ClosestEnemy != nullptr)
	{
		FVector Direction = ClosestEnemy->GetActorLocation() - GetActorLocation();
		FRotator TargetRotation = Direction.Rotation();
		FRotator CurrentRotation = GetController()->GetControlRotation();
		float InterpSpeed = 0.1f;
		FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, InterpSpeed);

		GetController()->SetControlRotation(NewRotation);
		SetTargetedEnemy(ClosestEnemy);
	}
	else
	{
		SetTargetedEnemy(nullptr);
	}
}

bool AC_Warrior::IsTargetLocked() const
{
	return TargetedEnemy != nullptr;
}

AC_Unit* AC_Warrior::GetTargetedEnemy() const
{
	return TargetedEnemy;
}

void AC_Warrior::SetTargetedEnemy(AC_Unit* NewTarget)
{
	TargetedEnemy = NewTarget;
}

void AC_Warrior::QSkillUp()
{
	//if (bIsAttacking)
		//return;
	//StartCameraShake();

	SwordQCooldown.bIsSkillActive = true;

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQSIcon(SwordQCooldown.bIsSkillActive);
	}

	
	if (GetWeaponType() == EWeaponState::SWORD)
	{
		//SetRotationToCameraDirection();
		RotateTowardsRaycastHit();

		float QSkillMontageDuration = 0.0f;
		if (QAttackSkillMontage)
		{
			QSkillMontageDuration = PlayAnimMontage(QAttackSkillMontage);
		}
	} 
	
	
}


void AC_Warrior::StopNiagaraEffect(UNiagaraComponent* NiagaraComponent)
{
	if (NiagaraComponent && !NiagaraComponent->IsBeingDestroyed())
	{
		NiagaraComponent->Deactivate();
	}
}

void AC_Warrior::ActivateGhostTrail()
{
	if (QAttackSkill_Niagara)
	{
		QSkillComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			QAttackSkill_Niagara,
			GetMesh(),
			FName(TEXT("None")),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

	}
	if (QSkillComponent)
	{
	

		FVector ComponentLocation = QSkillComponent->GetComponentLocation();
	}

}

void AC_Warrior::DeactivateGhostTrail()
{
	if (QSkillComponent && !QSkillComponent->IsBeingDestroyed())
	{
		QSkillComponent->Deactivate();
	}
}

void AC_Warrior::ActivateParticle()
{
	FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 280.0f); 
	SpawnLocation.Z -= 80.0f;
	FRotator SpawnRotation = GetActorRotation();

	QSkillParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		QSkillEffect,
		SpawnLocation,
		SpawnRotation
	);

	if (QSkillParticleComponent)
	{
		QSkillParticleComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 0.5f));
	}

	if (QSkillSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), QSkillSound, this->GetActorLocation());
	}

}

void AC_Warrior::DeactiveParticle()
{
	if (QSkillParticleComponent && !QSkillParticleComponent->IsBeingDestroyed())
	{
		QSkillParticleComponent->Deactivate();
	}

}

void AC_Warrior::TotalQSkill()
{
	if (CurMp < 20)
		return;

	if (bIsAttacking)
		return;

	if (GetWeaponType() == EWeaponState::SWORD)
	{
		if (SwordQCooldown.bIsSkillActive)
			return;

		QSkillUp();
		CurMp -= 20;
	}
	if (GetWeaponType() == EWeaponState::BOW)
	{
		if (BowQCooldown.bIsSkillActive)
			return;

		JumpBack();
		CurMp -= 20;
	}
}

void AC_Warrior::TotalESkill()
{
	if (CurMp < 20)
		return;

	if (bIsAttacking)
		return;

	if (GetWeaponType() == EWeaponState::SWORD)
	{
		if (SwordECooldown.bIsSkillActive)
			return;

		ActivateSkillCamera();
		ESkillStart();
		CurMp -= 20;
	}
	if (GetWeaponType() == EWeaponState::BOW)
	{
		if (BowECooldown.bIsSkillActive)
			return;

		if (bIsBowESkillAcitvated)
			return;

		CurMp -= 20;
		SetBowESkillArea();
		SpawnArrows();
	}
}

void AC_Warrior::SetQSkillCollider()
{
	QSkillCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("QSkillCollider"));
	QSkillCollider->SetupAttachment(RootComponent);
	QSkillCollider->InitBoxExtent(FVector(500.0f, 500.0f, 500.0f));
	QSkillCollider->SetRelativeLocation(FVector(242.0f, 0.0f, 77.0f));
	QSkillCollider->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.325f));
	QSkillCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void AC_Warrior::ActivateQSkillCollider()
{
	QSkillCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AC_Warrior::DeactiveQSkillCollider()
{
	QSkillCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Victims.Empty();
}

void AC_Warrior::SetTargetEnemyAir(AC_Enemy* NewTarget)
{
	if (NewTarget)
	{
		TargetEnemies.AddUnique(NewTarget); 
	}
}


void AC_Warrior::LaunchWarriorToAir()
{
	bool bHasValidTargets = false;
	for (AC_Enemy* Enemy : TargetEnemies)
	{
		if (Enemy != nullptr)
		{
			bHasValidTargets = true;
			break;
		}
	}

	if (bHasValidTargets)
	{
		FVector LaunchVelocity(0, 0, 900);
		LaunchCharacter(LaunchVelocity, true, true);
		bIsWarriorLaunched = true;

		if (ESkillDash_Niagara)
		{
			ESkillDashComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ESkillDash_Niagara,
				GetMesh(),
				FName(TEXT("None")),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}
		if (ESkillDashComponent)
		{
			ESkillDashComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

			FName ParameterName = FName(TEXT("YourParameterName"));
			float ParameterValue = 1.0f;
			ESkillDashComponent->SetFloatParameter(ParameterName, ParameterValue);
		}
	}
	
}

void AC_Warrior::WarriorBounce()
{
	FVector LaunchVelocity(0, 0, 400);
	LaunchCharacter(LaunchVelocity, true, true);
}

void AC_Warrior::MakeBounceInAir()
{
	for (AC_Enemy* Enemy : TargetEnemies)
	{
		if (Enemy)
		{
			Enemy->EnemyBounce();
		}
	}
	WarriorBounce();
}

void AC_Warrior::WarriorApplyForce()
{
	FVector Force(0, 0, -1000);
	LaunchCharacter(Force, true, true);
}

void AC_Warrior::AfterQComboFinish()
{
	for (AC_Enemy* Enemy : TargetEnemies)
	{
		if (Enemy)
		{
			FVector Force(0, 0, -3000);
			Enemy->ApplyForce(Force);
		}
	}
	WarriorApplyForce();
	TargetEnemies.Empty();

	QSkillCameraShake();

	StartQSSkillTimer(6.0f);
	UKismetSystemLibrary::K2_SetTimer(this, "ResetSwordQSkill", 6.0f, false);
}

void AC_Warrior::SetQComboFinishMontageSpeed(float speed)
{
	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Sword* Sword = Cast<AC_Sword>(CurrentWeapon);
		if (Sword)
		{
			GetMesh()->GetAnimInstance()->Montage_SetPlayRate(Sword->GetQSkillComboFinishMontage(),speed);
		}
	}

}

void AC_Warrior::PrintCurrentMontagePlayRate()
{
	
}

void AC_Warrior::QSKillComboStart()
{
	if (!bIsWarriorLaunched)
	{
		StartQSSkillTimer(3.0f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetSwordQSkill", 3.0f, false);

		return;
	}

	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Sword* Sword = Cast<AC_Sword>(CurrentWeapon);
		if (Sword)
		{
			Sword->QSkillComboStart();
			bIsWarriorLaunched = false;
		}
	}

	//PrintCurrentMontagePlayRate();
}

void AC_Warrior::QSkillComboFinish()
{

	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Sword* Sword = Cast<AC_Sword>(CurrentWeapon);
		if (Sword)
		{
			Sword->QSkillComboFinish();

			//bIsWarriorLaunched = false;
		}
	}
	
	//PrintCurrentMontagePlayRate();
}

void AC_Warrior::ESkillStart()
{
	if (ESkillDashCount != 0)
		return;

	//if (bIsAttacking)
		//return;

	SwordECooldown.bIsSkillActive = true;

	ActivateTrailEffect();

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateESIcon(SwordECooldown.bIsSkillActive);
	}

	if (GetWeaponType() == EWeaponState::SWORD)
	{
		SetRotationToCameraDirection();
		SpawnESkillColliderBox();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		ESkillDash();
	}

}

void AC_Warrior::ESkillDash()
{
	if (ESkillDashCount >= 5)
	{
		ESkillDashCount = 0;
		LaunchAfterESkill();
		DeactivateTrailEffect();
		return;
	}
	
	FRotator CurrentRotation = GetActorRotation();
	FRotator NewRotation;

	float RotationAngle = ESkillDashCount == 0 ? -25.0f : 144.0f;
	NewRotation = CurrentRotation + FRotator(0, RotationAngle, 0);


	PreDashRotation = GetActorRotation();
	SetActorRotation(NewRotation);

	FVector DashDirection = GetActorForwardVector();
	float DashDistance;
	if (!GetCharacterMovement()->IsFalling())
		DashDistance = 12000.0f;
	else
		DashDistance = 3000.0f;

	FVector DashVelocity = DashDirection * DashDistance;

	if (DashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DashSound, GetActorLocation());
	}
	
	if (ESkillDash_Niagara)
	{
		ESkillDashComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ESkillDash_Niagara,
			GetMesh(),
			FName(TEXT("None")),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
	if (ESkillDashComponent)
	{
		ESkillDashComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f)); 

		FName ParameterName = FName(TEXT("YourParameterName")); 
		float ParameterValue = 1.0f; 
		ESkillDashComponent->SetFloatParameter(ParameterName, ParameterValue);
	}

	if (ESkill_GhostTrail_Niagara)
	{
		ESkillGhostNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ESkill_GhostTrail_Niagara,
			GetMesh(),
			FName(TEXT("None")),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	if (ESkillColliderBox)
	{
		ESkillColliderBox->ActivateCollision();
	}

	LaunchCharacter(DashVelocity, true, true);

	UKismetSystemLibrary::K2_SetTimer(this, "ESkillDash", 0.18f, false);

	UKismetSystemLibrary::K2_SetTimer(this, "DisableColliderCollision", 0.1f, false);

	ESkillDashCount++;

}

void AC_Warrior::ESkillCombo()
{

	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Sword* Sword = Cast<AC_Sword>(CurrentWeapon);
		if (Sword)
		{
			Sword->ESkillComboStart();

			UKismetSystemLibrary::K2_SetTimer(this, "ESKillDash", Sword->GetESkillStartMontage()->GetPlayLength() * 0.9f, false);
		}
	}
}

void AC_Warrior::SpawnESkillColliderBox()
{
	FVector SpawnOffset(600.0f, 100.0f, 15.0f);

	FVector SpawnLocation = GetActorLocation() + GetActorRotation().RotateVector(SpawnOffset);
	SpawnLocation.Z += 50.0f;
	FRotator SpawnRotation = GetActorRotation();

	ESkillColliderBox = GetWorld()->SpawnActor<AC_ESkillColliderBox>(BPESkillColliderBox, SpawnLocation, SpawnRotation);

	if (ESkillColliderBox)
	{
		ESkillColliderBox->SetOwnerController(GetController());
		ESkillColliderBox->SetOwner(this);

		ESkillColliderBox->BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		ESkillColliderBox->BoxCollider->SetWorldScale3D(FVector(18.0f, 18.0f, 3.0f));

		ESkillColliderBox->BoxCollider->SetHiddenInGame(true);

		BoxExtent = ESkillColliderBox->BoxCollider->GetScaledBoxExtent();
		BoxCenter = ESkillColliderBox->BoxCollider->GetComponentLocation();
	}
}

void AC_Warrior::DestroyESkillColliderBox()
{
	if (ESkillColliderBox)
	{
		ESkillColliderBox->Destroy();
		ESkillColliderBox = nullptr;
	}

	DeactivateSkillCamera();
}

void AC_Warrior::DisableColliderCollision()
{
	if (ESkillColliderBox)
	{
		ESkillColliderBox->DeacivateCollision();
	}
}

void AC_Warrior::LaunchAfterESkill()
{
	FVector TargetLocation;
	
	TargetLocation = (ESkillColliderBox != nullptr) ? ESkillColliderBox->GetActorLocation() : GetActorLocation();

	FVector LaunchDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = LaunchDirection.Rotation();
	SetActorRotation(NewRotation);

	FVector DashDirection = GetActorForwardVector();
	float DashDistance = 3000.0f;
	
	FVector DashVelocity = DashDirection * DashDistance;

	LaunchCharacter(DashVelocity, true, true);

	UKismetSystemLibrary::K2_SetTimer(this, "LandAfterESkill", 0.3f, false);

	if (ESkillColliderBox)
	{
		ESkillColliderBox->DeacivateCollision();
	}
}

void AC_Warrior::LandAfterESkill()
{
	if (bIsAttacking)
		return;

	if (ESKillLandMontage)
	{
		PlayAnimMontage(ESKillLandMontage);

		FVector LaunchVelocity(0, 0, 900);
		LaunchCharacter(LaunchVelocity, true, true);
		ESkillFinishEffect();
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		if (ESkill_GhostTrail_Niagara)
		{
			ESkillGhostNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ESkill_GhostTrail_Niagara,
				GetMesh(),
				FName(TEXT("None")),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}

	}

	UKismetSystemLibrary::K2_SetTimer(this, "ESkillLastGhostTrail", 0.3f, false);

}

void AC_Warrior::ESkillLastGhostTrail()
{

	if (ESkill_GhostTrail_Niagara)
	{
		ESkillGhostNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ESkill_GhostTrail_Niagara,
			GetMesh(),
			FName(TEXT("None")),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

}

void AC_Warrior::ESkillFinishEffect()
{


	if (ESkillFloor)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FVector Scale = FVector(5.0f, 5.0f, 5.0f);
		ESkillFloorComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ESkillFloor, SpawnLocation, SpawnRotation, Scale);

		UKismetSystemLibrary::K2_SetTimer(this, "DeactivateESkillLandEffect", 2.0f, false);
	}

	
	if (ESkillAir)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z -= 10.0f;
		FRotator SpawnRotation = GetActorRotation();
		FVector Scale = FVector(1.0f, 1.0f, 0.6f);
		ESkillAirComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ESkillAir, SpawnLocation, SpawnRotation, Scale);

		UKismetSystemLibrary::K2_SetTimer(this, "DeactivateESkillAirEffect", 0.2f, false);

	}
	
	if (ESkillCenterThunder)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FVector Scale = FVector(4.0f, 4.0f, 4.0f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ESkillCenterThunder, SpawnLocation, SpawnRotation, Scale);

		if (MainThunderSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), MainThunderSound, this->GetActorLocation());
		}

	}

	ESKillSideThunder();
}

void AC_Warrior::DeactivateESkillLandEffect()
{
	ESkillFloorComponent->Deactivate();
}

void AC_Warrior::DeactivateESkillAirEffect()
{
	ESkillAirComponent->Deactivate();
}

void AC_Warrior::ESKillSideThunder()
{
	if (ESkillColliderBox)
	{

		ESkillColliderBox->GetCanLaunchByESkill() = true;
		ESkillColliderBox->ActivateCollision();

		GetWorldTimerManager().SetTimer(TimerHandle, this, &AC_Warrior::SpawnRandomThunder, 0.2f, true, 0.0f);
	}
}

void AC_Warrior::SpawnRandomThunder()
{
	if (ThunderSpawnCount > 10)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);

		if (ESkillColliderBox)
		{
			ESkillColliderBox->DeacivateCollision();
			ESkillColliderBox->GetCanLaunchByESkill() = false;

		}

		DestroyESkillColliderBox();
		ThunderSpawnCount = 0;

		StartESSkillTimer(9.0f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetSwordESkill", 9.0f, false);

		return;
	}

	FVector RandomLocation = BoxCenter + FMath::RandPointInBox(FBox(-BoxExtent, BoxExtent));

	if (ESkillSideThunder)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ESkillSideThunder, RandomLocation, FRotator::ZeroRotator);
		ThunderSpawnCount++;
	}

	if (SideThunderSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SideThunderSound, this->GetActorLocation());
	}
}

void AC_Warrior::ActivateSkillCamera()
{
	if (!SkillCamera)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SkillCamera = GetWorld()->SpawnActor<AC_SkillCamera>(AC_SkillCamera::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	if (SkillCamera)
	{
		FVector  CPlayerLocation = GetActorLocation();
		FRotator CPlayerRotation = GetActorRotation();

		FVector LeftOffset = -CPlayerRotation.RotateVector(FVector::RightVector) * 800;
		FVector BackOffset = -CPlayerRotation.RotateVector(FVector::ForwardVector) * 800;
		FVector UpOffset = FVector::UpVector * 1000;

		FVector FinalPosition = CPlayerLocation + LeftOffset + BackOffset + UpOffset;

		FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(FinalPosition, CPlayerLocation);

		SkillCamera->SetActorLocationAndRotation(FinalPosition, CameraRotation);

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->SetViewTargetWithBlend(SkillCamera, 1.0f); 
		}
	}
}

void AC_Warrior::DeactivateSkillCamera()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && Camera) 
	{
		PC->SetViewTargetWithBlend(this, 1.0f); 
	}
}

FVector AC_Warrior::CalculateCenterOfEnemies(const TArray<AC_Enemy*>& Enemies)
{
	FVector SumLocation = FVector::ZeroVector;
	for (AC_Enemy* Enemy : Enemies)
	{
		if (Enemy)
		{
			SumLocation += Enemy->GetActorLocation();
		}
	}
	return Enemies.Num() > 0 ? SumLocation / Enemies.Num() : FVector::ZeroVector;
}

void AC_Warrior::JumpBack()
{
	BowQCooldown.bIsSkillActive = true;

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQBIcon(BowQCooldown.bIsSkillActive);
	}


	if (QJumpBackMontage)
	{
		PlayAnimMontage(QJumpBackMontage, 2.5f);
	}
	FVector BackwardVector = -GetActorForwardVector(); 
	FVector LaunchVelocity = BackwardVector * 500 + FVector(0, 0, 500); 
	LaunchCharacter(LaunchVelocity, true, true); 
}

void AC_Warrior::ShootArrowAfterJumpBack()
{
	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Bow* bow = Cast<AC_Bow>(CurrentWeapon);
		if (bow)
		{
			bow->PlayFireArrow();
		}
	}


	StartQBSkillTimer(4.0f);
	UKismetSystemLibrary::K2_SetTimer(this, "ResetBowQSkill", 4.0f, false);

}

void AC_Warrior::SetBowESkillArea()
{
	FVector SpawnOffset(1000.0f, 100.0f, 15.0f);

	FVector SpawnLocation = GetActorLocation() + GetActorRotation().RotateVector(SpawnOffset);
	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BowESkillArea = GetWorld()->SpawnActor<AC_ESkillColliderBoxBow>(AC_ESkillColliderBoxBow::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

	if (BowESkillArea)
	{
		BowESkillArea->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		BowESkillArea->CollisionBox->SetWorldScale3D(FVector(25.0f, 25.0f, 3.0f));

		BowESkillArea->CollisionBox->SetHiddenInGame(true);

		BowBoxExtent = BowESkillArea->CollisionBox->GetScaledBoxExtent();
		BowBoxCenter = BowESkillArea->CollisionBox->GetComponentLocation();
	}

	BowECooldown.bIsSkillActive = true;

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateEBIcon(BowECooldown.bIsSkillActive);
	}


}

void AC_Warrior::SpawnArrows()
{
	if (!BowESkillArea || ArrowSpawnCount >= 80)
	{
		ArrowSpawnCount = 0; 

		StartEBSkillTimer(7.0f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetBowESkill", 7.0f, false);

		return; 
	}



	FVector RandomLocation = FMath::RandPointInBox(FBox(BowBoxCenter - BowBoxExtent, BowBoxCenter + BowBoxExtent));
	RandomLocation.Z += 500;
	FRotator SpawnRotation = FRotator::ZeroRotator; 

	AC_FallingArrow* SpawnedArrow = GetWorld()->SpawnActor<AC_FallingArrow>(FallingArrow, RandomLocation, SpawnRotation);
	if (SpawnedArrow)
	{
		SpawnedArrows.Add(SpawnedArrow);
		LastArrowFallTime = GetWorld()->TimeSeconds + 1.0f;

		SpawnedArrow->SetOwnerController(GetController());
		SpawnedArrow->ActivateCollision();

		if (SpawnedArrow->GetNiagaraComponent())
		{
			SpawnedArrow->GetNiagaraComponent()->Activate(true);
		}
		
		if (SpawnedArrow->GetCapsuleComponent())
		{
			SpawnedArrow->GetCapsuleComponent()->SetHiddenInGame(true);
		}

		FTimerHandle UnusedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [SpawnedArrow]() { SpawnedArrow->SetIsFallingTrue(); }, 1.0f, false);
	}

	if (ESkillBowFloor && ArrowSpawnCount == 0) 
	{
		FVector FloorSpawnLocation = BowBoxCenter; 
		FloorSpawnLocation.Z -= 100;
		FRotator FloorSpawnRotation = FRotator::ZeroRotator;
		FVector Scale = FVector(7.0f, 7.0f, 1.0f); 

		ESkillFloorBowComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ESkillBowFloor, FloorSpawnLocation, FloorSpawnRotation, Scale);
		//ESkillFloorBowComponent->SetHiddenInSceneCapture(true);
		
	}

	if (ESkillBowFloor && ArrowSpawnCount == 79)
	{
		UKismetSystemLibrary::K2_SetTimer(this, "DeactivateESkillBowLandEffect", 1.0f, false);
	}


	ArrowSpawnCount++; 

	GetWorld()->GetTimerManager().SetTimer(ArrowTimerHandle, this, &AC_Warrior::SpawnArrows, 0.1f, false);
}

void AC_Warrior::DestroyESkillArea()
{
	if (BowESkillArea)
	{
		BowESkillArea->Destroy();
		BowESkillArea = nullptr;
	}
	bIsBowESkillAcitvated = false;
}

void AC_Warrior::DeactivateESkillBowLandEffect()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "deactivatebowe");

	ESkillFloorBowComponent->Deactivate();
}


void AC_Warrior::Kick()
{
	if (bIsAttacking)
		return;
	if (bIsKicking)
		return;

	SetRotationToCameraDirection();

	bIsKicking = true;

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateKickCoolTime(bIsKicking);
	}

	if (KickMontage)
	{
		PlayAnimMontage(KickMontage);
	}

	UKismetSystemLibrary::K2_SetTimer(this, "ResetKick", 1.0f, false);
}

void AC_Warrior::SetKickCollider()
{
	KickCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("KickCollider"));
	KickCollider->SetupAttachment(GetMesh(), FName("RightFoot")); 
	KickCollider->SetRelativeLocation(FVector(0.181818, -0.0f, 0.0f));
	KickCollider->SetRelativeRotation(FRotator(34.491565, 10.573044, -29.812906));
	KickCollider->SetRelativeScale3D(FVector(0.02f, 0.02f, 0.02f));
	KickCollider->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	KickCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void AC_Warrior::ActivateKickCollider()
{
	KickCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AC_Warrior::DeactivateKickCollider()
{
	KickCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Victims.Empty();
}

void AC_Warrior::ResetKick()
{
	bIsKicking = false;

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateKickCoolTime(bIsKicking);
	}
}

void AC_Warrior::OnLeftMouseDown()
{
	bIsLeftMouseDown = true;
	LeftMouseHoldTime = 0.0f;
	AdditionalSpeed = 0.0f;
	bIsCharging = true;
}

void AC_Warrior::OnLeftMouseUp()
{
	bIsLeftMouseDown = false;

	AdditionalSpeed = CalculateAdditionalSpeed(LeftMouseHoldTime);

	LeftMouseHoldTime = 0.0f;
	bIsCharging = false;

	if (BowChargingComponent)
	{
		BowChargingComponent->DeactivateSystem();  
		BowChargingComponent->DestroyComponent(); 
		BowChargingComponent = nullptr;  
	}
}

void AC_Warrior::ActivateChargingEffect()
{
	if (!BowChargingComponent && BowChargingParticle)
	{
		BowChargingComponent = UGameplayStatics::SpawnEmitterAttached(BowChargingParticle, GetMesh(), FName("ChargingSocket"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	}

	if (BowChargingComponent)
	{
		float HoldTime = LeftMouseHoldTime;  // 현재 마우스를 누른 시간
		float ScaleFactor = FMath::Clamp((HoldTime) / 3.0f, 0.0f, 0.3f);  

		FVector NewScale = FVector(0.1f + ScaleFactor, 0.1f + ScaleFactor, 0.1f + ScaleFactor);  
		BowChargingComponent->SetWorldScale3D(NewScale);
	}

}

void AC_Warrior::PauseBowAttackMontage()
{
	if (bIsLeftMouseDown)
	{
		AC_Weapon* CurrentWeapon = GetCurrentWeapon();
		if (CurrentWeapon)
		{
			AC_Bow* bow = Cast<AC_Bow>(CurrentWeapon);
			if (bow)
			{
				bIsBowMontageStopped = true;
				GetMesh()->GetAnimInstance()->Montage_Pause(bow->GetBowFireMontage());
			}
		}
	}
}

void AC_Warrior::ResumeBowAttackMontage()
{
	if (bIsBowMontageStopped)
	{
		AC_Weapon* CurrentWeapon = GetCurrentWeapon();
		if (CurrentWeapon)
		{
			AC_Bow* bow = Cast<AC_Bow>(CurrentWeapon);
			if (bow)
			{
				bIsBowMontageStopped = false;
				GetMesh()->GetAnimInstance()->Montage_Resume(bow->GetBowFireMontage());
			}
		}
	}

}

void AC_Warrior::AppearArrow()
{
	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Bow* bow = Cast<AC_Bow>(CurrentWeapon);
		if (bow)
		{
			bow->AppearArrow();
			if (BowStartSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BowStartSound, this->GetActorLocation());
			}
		}
	}
}

void AC_Warrior::FireArrow()
{
	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Bow* bow = Cast<AC_Bow>(CurrentWeapon);
		if (bow)
		{
			bow->FireArrow();
			if (BowReleaseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BowReleaseSound, this->GetActorLocation());
			}
			if (BowFireSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BowFireSound, this->GetActorLocation());
			}
		}
	}
}

float AC_Warrior::CalculateAdditionalSpeed(float HoldTime)
{
	float MaxHoldTime = 3.0f;  
	float MaxAdditionalSpeed = 1000.0f; 

	float HoldRatio = FMath::Min(HoldTime / MaxHoldTime, 1.0f);

	return HoldRatio * MaxAdditionalSpeed;
}

void AC_Warrior::ShowTrajectory()
{
	if (TrajectoryComponent)
	{
		TrajectoryComponent->DestroyComponent();
		TrajectoryComponent = nullptr;
	}

	FPredictProjectilePathParams PredictParams;
	FPredictProjectilePathResult PredictResult;

	FVector StartLocation = GetMesh()->GetSocketLocation("ProjectileSocket");
	PredictParams.StartLocation = StartLocation;

	FVector HitLocation;
	if (PerformRaycastFromCamera(HitLocation))
	{
		FVector FireDirection = (HitLocation - StartLocation).GetSafeNormal();
		FireDirection.Z += 0.2f; 
		PredictParams.LaunchVelocity = FireDirection * (2000.0f + CalculateAdditionalSpeed(LeftMouseHoldTime));
	}
	else
	{
		PredictParams.LaunchVelocity = GetActorForwardVector() * (2000.0f + CalculateAdditionalSpeed(LeftMouseHoldTime));
	}


	PredictParams.bTraceWithCollision = false;
	PredictParams.ProjectileRadius = 10.0f; 
	PredictParams.MaxSimTime = 20.0f; 
	PredictParams.SimFrequency = 15.0f;
	PredictParams.OverrideGravityZ = -981.0f; 

	bool bHit = UGameplayStatics::PredictProjectilePath(
		this,
		PredictParams,
		PredictResult
	);

	int32 PathDataCount = PredictResult.PathData.Num();
	int32 HalfWayIndex = PathDataCount / 2;

	for (int32 i = 0; i < PredictResult.PathData.Num()-1; i++)
	{
		DrawDebugLine(
			GetWorld(),
			PredictResult.PathData[i].Location,
			PredictResult.PathData[i + 1].Location,
			FColor(173, 216, 230),
			false,
			-1.f,
			(uint8)'\000',
			5.0f
		);
	}
	
	if (UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult))
	{
		
		const FPredictProjectilePathPointData& LastPoint = PredictResult.PathData.Last();
		TrajectoryComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TrajectoryNiagara,
			LastPoint.Location,
			FRotator::ZeroRotator
		);

	
	}
}

void AC_Warrior::ZoomIn()
{
	if (bWorldMapOn)
		return;

	if (WeaponType == EWeaponState::BOW)
	{
		BowCamera->SetActive(true);
		Camera->SetActive(false);
		bIsBowCameraActive = true;
		
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->SetViewTargetWithBlend(this, 2.5f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, true);
		}
	}
}

void AC_Warrior::ZoomOut()
{
	if (WeaponType == EWeaponState::BOW)
	{
		Camera->SetActive(true);
		BowCamera->SetActive(false);
		bIsBowCameraActive = false;

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->SetViewTargetWithBlend(this, 2.5f, EViewTargetBlendFunction::VTBlend_Linear, 0.0f, true);
		}
	}
}

void AC_Warrior::BlockOn()
{
	StartBlocking();
}

void AC_Warrior::BlockOff()
{
	EndBlocking();
}

void AC_Warrior::TestSample()
{
	//ApplyStun(5.0f);
	//StunnedBarOn();
	//KnuckBack();
	//Freezed();
	//StunnedCameraShake();
	//QSkillCameraShake();
	//SucceedBlockCameraShake();
	ActiveLevelUpParticle();
}

void AC_Warrior::StunnedCameraShake()
{
	if (CameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeClass);
	}
}

void AC_Warrior::QSkillCameraShake()
{
	if (QSkillCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(QSkillCameraShakeClass);
	}
}

void AC_Warrior::SucceedBlockCameraShake()
{
	if (SucceedBlockCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(SucceedBlockCameraShakeClass);
	}
}


void AC_Warrior::UpdateWeaponIcons()
{

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateFistIcon(WeaponType == EWeaponState::UNARMED);
		CachedHud->PlayerUIInstance->UpdateSwordIcon(WeaponType == EWeaponState::SWORD);
		CachedHud->PlayerUIInstance->UpdateBowIcon(WeaponType == EWeaponState::BOW);
	}
}

void AC_Warrior::RecoverMP(int32 Amount)
{
	CurMp += Amount;

	CurMp = FMath::Clamp(CurMp, 0, MaxMp);
}

void AC_Warrior::AddExp(int32 Amount)
{
	CurExp += Amount;
}

void AC_Warrior::LevelUp()
{
	ActiveLevelUpParticle();

	level++;
	CurExp -= MaxExp;
	MaxExp += 50;

	MaxHp += 30;
	CurHp = MaxHp;

	MaxMp += 10;
	CurMp = MaxMp;
}

void AC_Warrior::ActiveLevelUpParticle()
{
	if (LevelUpParticle) 
	{
		FVector Location = GetActorLocation();
		Location.Z -= 120.0f;
		FRotator Rotation = GetActorRotation();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LevelUpParticle, Location, Rotation, true);
	}
}

void AC_Warrior::StartQSSkillTimer(float cooltimer)
{
	SwordQCooldown.CooldownDuration = cooltimer;
	SwordQCooldown.CooldownStartTime = GetWorld()->GetTimeSeconds();

	UKismetSystemLibrary::K2_SetTimer(this, "UpdateQSCooldown", 0.1f, true);

	SwordQCooldown.bIsTimerActive = true;
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQSTimerActive(SwordQCooldown.bIsTimerActive);
	}

}

void AC_Warrior::UpdateQSCooldown()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - SwordQCooldown.CooldownStartTime;
	float TimeRemaining = SwordQCooldown.CooldownDuration - TimeElapsed;

	if (TimeRemaining <= 0.0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SwordQCooldown.CooldownTimerHandler);
		TimeRemaining = 0.0;
	}

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQSCoolTime(TimeRemaining);
	}

}

void AC_Warrior::ResetSwordQSkill()
{
	SwordQCooldown.bIsSkillActive = false;
	SwordQCooldown.bIsTimerActive = false;
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQSIcon(SwordQCooldown.bIsSkillActive);
		CachedHud->PlayerUIInstance->UpdateQSTimerActive(SwordQCooldown.bIsTimerActive);
	}
}

void AC_Warrior::StartESSkillTimer(float cooltimer)
{
	SwordECooldown.CooldownDuration = cooltimer;
	SwordECooldown.CooldownStartTime = GetWorld()->GetTimeSeconds();

	UKismetSystemLibrary::K2_SetTimer(this, "UpdateESCooldown", 0.1f, true);

	SwordECooldown.bIsTimerActive = true;
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateESTimerActive(SwordECooldown.bIsTimerActive);
	}
}

void AC_Warrior::UpdateESCooldown()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - SwordECooldown.CooldownStartTime;
	float TimeRemaining = SwordECooldown.CooldownDuration - TimeElapsed;

	if (TimeRemaining <= 0.0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SwordECooldown.CooldownTimerHandler);
		TimeRemaining = 0.0;
	}

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateESCoolTime(TimeRemaining);
	}
}

void AC_Warrior::ResetSwordESkill()
{
	SwordECooldown.bIsSkillActive = false;
	SwordECooldown.bIsTimerActive = false;
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateESIcon(SwordECooldown.bIsSkillActive);
		CachedHud->PlayerUIInstance->UpdateESTimerActive(SwordECooldown.bIsTimerActive);
	}
}

void AC_Warrior::StartQBSkillTimer(float cooltimer)
{
	BowQCooldown.CooldownDuration = cooltimer;
	BowQCooldown.CooldownStartTime = GetWorld()->GetTimeSeconds();

	UKismetSystemLibrary::K2_SetTimer(this, "UpdateQBCooldown", 0.1f, true);

	BowQCooldown.bIsTimerActive = true;
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQBTimerActive(BowQCooldown.bIsTimerActive);
	}
}

void AC_Warrior::UpdateQBCooldown()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - BowQCooldown.CooldownStartTime;
	float TimeRemaining = BowQCooldown.CooldownDuration - TimeElapsed;

	if (TimeRemaining <= 0.0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BowQCooldown.CooldownTimerHandler);
		TimeRemaining = 0.0;
	}

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQBCoolTime(TimeRemaining);
	}
}

void AC_Warrior::ResetBowQSkill()
{
	BowQCooldown.bIsSkillActive = false;
	BowQCooldown.bIsTimerActive = false;

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQBIcon(BowQCooldown.bIsSkillActive);
		CachedHud->PlayerUIInstance->UpdateQBTimerActive(BowQCooldown.bIsTimerActive);
	}
}

void AC_Warrior::StartEBSkillTimer(float cooltimer)
{
	BowECooldown.CooldownDuration = cooltimer;
	BowECooldown.CooldownStartTime = GetWorld()->GetTimeSeconds();

	UKismetSystemLibrary::K2_SetTimer(this, "UpdateEBCooldown", 0.1f, true);

	BowECooldown.bIsTimerActive = true;
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateEBTimerActive(BowECooldown.bIsTimerActive);
	}
}

void AC_Warrior::UpdateEBCooldown()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - BowECooldown.CooldownStartTime;
	float TimeRemaining = BowECooldown.CooldownDuration - TimeElapsed;

	if (TimeRemaining <= 0.0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BowECooldown.CooldownTimerHandler);
		TimeRemaining = 0.0;
	}

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateEBCoolTime(TimeRemaining);
	}
}

void AC_Warrior::ResetBowESkill()
{
	BowECooldown.bIsSkillActive = false;
	BowECooldown.bIsTimerActive = false;

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateEBIcon(BowECooldown.bIsSkillActive);
		CachedHud->PlayerUIInstance->UpdateEBTimerActive(BowECooldown.bIsTimerActive);
	}
}

void AC_Warrior::ResetAllSkill()
{
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateQSTimerActive(false);
		CachedHud->PlayerUIInstance->UpdateESTimerActive(false);
		CachedHud->PlayerUIInstance->UpdateQBTimerActive(false);
		CachedHud->PlayerUIInstance->UpdateEBTimerActive(false);
	}
}

void AC_Warrior::Freezed()
{
	bIsFreezed = true;

	/*
	if (FreezeNiagara)
	{
		FreezeComponent->SetAsset(FreezeNiagara);
		FreezeComponent->Activate(true);
	}
	*/
	
	if (FreezeNiagara)
	{
		FreezeComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			FreezeNiagara,
			GetMesh(),
			FName(TEXT("None")),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
	
	if (FreezeComponent)
	{
		FreezeComponent->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	}

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->IsFreezed(bIsFreezed);
	}


	GetWorld()->GetTimerManager().SetTimer(FreezeTimerHandle, this, &AC_Warrior::ResetFreezed, 1.5f, false);
}

void AC_Warrior::ResetFreezed()
{
	if (bIsFreezed == true)
	{
		bIsFreezed = false;
		if (CachedHud && CachedHud->PlayerUIInstance)
		{
			CachedHud->PlayerUIInstance->IsFreezed(bIsFreezed);
		}
	}
}

void AC_Warrior::StunnedBarOn()
{
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->IsStunned(true);
	}

	BarStunStartTime = GetWorld()->GetTimeSeconds();

	GetWorld()->GetTimerManager().SetTimer(StunnedTimerHandle, this, &AC_Warrior::UpdateStunnedBar, 0.1f, true);

	StunnedCameraShake();
}

void AC_Warrior::UpdateStunnedBar()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - BarStunStartTime;

	if (TimeElapsed >= StunDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(StunnedTimerHandle);
		CachedHud->PlayerUIInstance->IsStunned(false);
		ResetStun();
		//UKismetSystemLibrary::PrintString(GetWorld(), "finish");
	}

	CachedHud->PlayerUIInstance->UpdateStunBar(TimeElapsed, StunDuration);
}

FVector AC_Warrior::GetRandomPoint(float Radius) const
{
	if (Radius <= 0.0f)
	{
		return GetActorLocation(); 
	}

	float RandomAngle = FMath::RandRange(0.0f, 360.0f);
	float RandomRadius = FMath::RandRange(0.0f, Radius);
	FVector Direction = FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle)), FMath::Sin(FMath::DegreesToRadians(RandomAngle)), 0.0f);

	FVector Location = GetActorLocation() + Direction * RandomRadius;
	//FString strlocation = Location.ToString();

	FVector warriorlocation = this->GetActorLocation();
	//FString strwarriorlocation = warriorlocation.ToString();

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Random Point: %s"), *strlocation), true, true, FLinearColor::Yellow);
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Warrio Point: %s"),*strwarriorlocation), true, true, FLinearColor::Red);
	return GetActorLocation() + Direction * RandomRadius;
}

void AC_Warrior::PetKey()
{
	//SpawnPet();

	if (bIsPetSpawnDecalActive)
	{
		bIsPetSpawnDecalActive = false;
		bCanPetSpawnAreaMove = false;
		PetSpawnAreaInstance->Destroy();
		PetSpawnAreaInstance = nullptr;
	}
	else
	{
		FVector HitLocation;
		if (PerformRaycastFromCamera(HitLocation))
		{
			PetSpawnAreaInstance = GetWorld()->SpawnActor<AC_PetSpawn>(PetSpawnArea, HitLocation, FRotator::ZeroRotator);
			bIsPetSpawnDecalActive = true;
			bCanPetSpawnAreaMove = true;
			
		}
	}
}

void AC_Warrior::SpawnPet()
{
	bCanPetSpawnAreaMove = false;

	FVector Origin;
	if (PetSpawnAreaInstance)
	{
		Origin = PetSpawnAreaInstance->GetActorLocation();
		Origin.Z += 500.0f;
	}
	else
	{
		Origin = GetActorLocation();
	}
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AC_MousePet* pet = GetWorld()->SpawnActor<AC_MousePet>(mousePet, Origin, SpawnRotation, SpawnParams);

	pet->PetFloating();
	if (pet)
	{
		auto AIController = GetWorld()->SpawnActor<AC_PetAIController>(AC_PetAIController::StaticClass());
		if (AIController)
		{
			AIController->Possess(pet);
		}
	}

	/*
	if (PetSpawnAreaInstance)
	{
		PetSpawnAreaInstance->Destroy();
		PetSpawnAreaInstance = nullptr;
	}

	bIsPetSpawnDecalActive = false;
	*/

	UKismetSystemLibrary::K2_SetTimer(this, "DestroyPetSpawnArea", 2.0f, false);
}

void AC_Warrior::DestroyPetSpawnArea()
{
	if (PetSpawnAreaInstance)
	{
		PetSpawnAreaInstance->Destroy();
		PetSpawnAreaInstance = nullptr;
	}

	bIsPetSpawnDecalActive = false;

}

void AC_Warrior::MapChange()
{
	bWorldMapOn = !bWorldMapOn;
	bMiniMapOn = !bMiniMapOn;

	if (MiniMapCaptureComponent)
	{
		if (bWorldMapOn)
		{
			//MiniMapCaptureComponent->SetOrthoWidth(3000.0f);
			MiniMapCaptureComponent->OrthoWidth = 4000.0f;

			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->bEnableClickEvents = true;
				PlayerController->bEnableMouseOverEvents = true;

				FVector2D ViewportSize;
				GEngine->GameViewport->GetViewportSize(ViewportSize);
				float CenterX = ViewportSize.X / 2;
				float CenterY = ViewportSize.Y / 2;
				PlayerController->SetMouseLocation(CenterX, CenterY);

			}

			MiniMapCameraLocation = MiniMapCaptureComponent->GetComponentLocation();

		}
		else
		{
			//MiniMapCaptureComponent->SetOrthoWidth(1500.0f);
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->bEnableClickEvents = false;
				PlayerController->bEnableMouseOverEvents = false;
			}
			
			bIsDraggingMap = false;

			MiniMapCaptureComponent->OrthoWidth = 1500.0f;

			MiniMapCaptureComponent->SetWorldLocation(MiniMapCameraLocation);
		}
	}


	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->WorldMapOn(bWorldMapOn);
		CachedHud->PlayerUIInstance->MiniMapOn(bMiniMapOn);
	}
}

void AC_Warrior::BossMapSetting()
{
	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->WorldMapOn(false);
		CachedHud->PlayerUIInstance->MiniMapOn(false);
	}

}

void AC_Warrior::SavePlayerInstance()
{
	UC_PlayerInstance* playerInstance = Cast<UC_PlayerInstance>(GetGameInstance());
	if (playerInstance)
	{
		UC_PlayerInstance::Builder builder(playerInstance);
		builder.SetPlayerLevel(level)
			.SetPlayerCurExp(CurExp)
			.SetPlayerMaxExp(MaxExp)
			.SetPlayerCurHP(CurHp)
			.SetPlayerMaxHP(MaxHp)
			.SetPlayerCurMP(CurMp)
			.SetPlayerMaxMP(MaxMp)
			.Build();
	}
}

void AC_Warrior::GetPlayerInstance()
{
	UC_PlayerInstance* playerInstance = Cast<UC_PlayerInstance>(GetGameInstance());
	if (playerInstance)
	{
		CurExp = playerInstance->PlayerCurExp ;
		MaxExp = playerInstance->PlayerMaxExp ;
		level  = playerInstance->PlayerLevel  ;
		CurHp  = playerInstance->PlayerCurHP  ;
		MaxHp  = playerInstance->PlayerMaxHP  ;
		CurMp  = playerInstance->PlayerCurMP  ;
		MaxMp  = playerInstance->PlayerMaxMP  ;
	}

}

void AC_Warrior::ActivateTrailEffect()
{
	if (SwordTrailComponent)
	{
		SwordTrailComponent->SetPaused(false);
		SwordTrailComponent->SetVisibility(true);
	}
}

void AC_Warrior::DeactivateTrailEffect()
{
	if (SwordTrailComponent)
	{
		SwordTrailComponent->SetPaused(false);
		SwordTrailComponent->SetVisibility(false);
	}
}

void AC_Warrior::ActivateSpawnCamera()
{
	if (!SkillCamera)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SkillCamera = GetWorld()->SpawnActor<AC_SkillCamera>(AC_SkillCamera::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	if (SkillCamera)
	{
		FVector CPlayerLocation = GetActorLocation();
		FRotator CPlayerRotation = GetActorRotation();

		// 캐릭터의 정면을 바라보는 카메라 위치 계산
		FVector ForwardOffset = CPlayerRotation.RotateVector(FVector::ForwardVector) * 1000;
		FVector CameraLocation = CPlayerLocation + ForwardOffset + FVector(0, 0, 400); 

		FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, CPlayerLocation);

		SkillCamera->SetActorLocationAndRotation(CameraLocation, CameraRotation);

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->SetViewTargetWithBlend(SkillCamera, 0.5f);
		}
	}
}

void AC_Warrior::DeactivateSpawnCamera()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && SkillCamera)
	{
		PC->SetViewTargetWithBlend(this, 1.0f);
	}
}

void AC_Warrior::HideAll()
{
	for (auto& weapon : Weapons)
		weapon.Value->SetActorHiddenInGame(true);
}


void AC_Warrior::UnHideAll()
{
	for (auto& weapon : Weapons)
		weapon.Value->SetActorHiddenInGame(false);

}

void AC_Warrior::OnQSkillOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Enemy* HitEnemy = Cast<AC_Enemy>(OtherActor);

	if (HitEnemy == nullptr)
		return;

	if (Victims.Find(HitEnemy) >= 0)
		return;

	Victims.Add(HitEnemy);

	if (HitEnemy&&HitEnemy->CanBeHit())
	{
		SetTargetEnemyAir(HitEnemy);
		HitEnemy->LaunchIntoAirQSKill(); 

		UGameplayStatics::ApplyDamage(
			HitEnemy,
			10.0f,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
}

void AC_Warrior::OnKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Enemy* HitEnemy = Cast<AC_Enemy>(OtherActor);

	if (HitEnemy == nullptr)
		return;

	if (Victims.Find(HitEnemy) >= 0)
		return;

	Victims.Add(HitEnemy);

	if (HitEnemy && HitEnemy->CanBeHit())
	{
		FVector DirectionToEnemy = HitEnemy->GetActorLocation() - GetActorLocation();
		DirectionToEnemy.Z = 0; 

		HitEnemy->KnockBack(DirectionToEnemy);

		UGameplayStatics::ApplyDamage(
			HitEnemy,
			5.0f,
			GetController(),
			this,
			UDamageType::StaticClass()
		);

		if (KickSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), KickSound, this->GetActorLocation());
		}
	}
}

AC_Hud* AC_Warrior::GetHud() const
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		return Cast<AC_Hud>(PC->GetHUD());
	}
	return nullptr;
}

