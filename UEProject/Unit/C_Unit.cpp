// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/C_Unit.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/C_Sword.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Widget/WC_Name.h"
#include "Widget/WC_HpBar.h"
#include "Sound/SoundBase.h"
#include "Unit/Enemy/C_Enemy.h"
#include "Unit/Warrior/C_Warrior.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "EnemySpawnArea/C_EnemySpawnArea.h"


// Sets default values
AC_Unit::AC_Unit()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	NameWidget = CreateDefaultSubobject<UWidgetComponent>("NameWidget");
	NameWidget->SetupAttachment(GetMesh());

	HpWidget = CreateDefaultSubobject<UWidgetComponent>("HpWidget");
	HpWidget->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void AC_Unit::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters param;
	param.Owner = this;

	for (auto weaponClass : WeaponClasses)
	{

		AC_Weapon* weapon = GetWorld()->SpawnActor<AC_Weapon>(weaponClass.Value, param);

		weapon->AttachWeapon(GetMesh());

		Weapons.Add(weaponClass.Key, weapon);

	}

	if (IsValid(NameWidget)) 
	{
		if (NameWidget->GetUserWidgetObject())
		{
			Cast<UWC_Name>(NameWidget->GetUserWidgetObject())->SetName(FText::FromString(UKismetSystemLibrary::GetObjectName(this)));
		}
	}

	StrafeOff();

	UMaterialInterface* curMaterial = GetMesh()->GetMaterial(0);

	BodyMaterial = UMaterialInstanceDynamic::Create(curMaterial, this);

	BodyMaterial->SetVectorParameterValue("HitColor", FVector4(0, 0, 0, 1));

	GetMesh()->SetMaterial(0, BodyMaterial);


}

// Called every frame
void AC_Unit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(HpWidget))
	{
		if (CurHp < MaxHp&&CurHp >0.0f)
		{
			if (HpWidget->GetUserWidgetObject())
			{
				UWC_HpBar* HpBarWidget = Cast<UWC_HpBar>(HpWidget->GetUserWidgetObject());
				if (HpBarWidget)
				{
					HpBarWidget->UpdateHp(CurHp, MaxHp);
					HpBarWidget->SetVisibility(ESlateVisibility::Visible); 
				}
			}
		}
		else
		{
			if (HpWidget->GetUserWidgetObject())
			{
				UWC_HpBar* HpBarWidget = Cast<UWC_HpBar>(HpWidget->GetUserWidgetObject());
				if (HpBarWidget)
				{
					HpBarWidget->SetVisibility(ESlateVisibility::Hidden); 
				}
			}
		}
	}
}

// Called to bind functionality to input
void AC_Unit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AC_Unit::StrafeOn()
{
	bUseControllerRotationYaw = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AC_Unit::StrafeOff()
{
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AC_Unit::UnEquipWeapon()
{
	Weapons[WeaponType]->DetachWeapon();

	if (NextEquipWeapon == EWeaponState::UNARMED)
		WeaponType = EWeaponState::UNARMED;
	else
	{
		WeaponType = NextEquipWeapon;

		PlayAnimMontage(GetCurrentWeapon()->GetEquipMontage());
	}

//	StrafeOn();
}

void AC_Unit::EquipWeapon()
{
	Weapons[NextEquipWeapon]->AttachWeapon();

	NextEquipWeapon = EWeaponState::UNARMED;

//	if (WeaponType == EWeaponState::SWORD)
		//StrafeOff();
}

void AC_Unit::OnAttack()
{
	if (CurHp <= 0)
		return;

	if (WeaponType == EWeaponState::UNARMED)
		return;

	//if (bIsStunned)
		//return;

	//bIsLoop = true;

	Weapons[WeaponType]->Attack();
}

void AC_Unit::OffAttack()
{
	//bIsLoop = false;
}

void AC_Unit::EnableAttack()
{
	bIsAttacking = false;
	bCanMove = true;
}

void AC_Unit::DisableAttack()
{
	bIsAttacking = true;
}

void AC_Unit::EnableCombo()
{
	Cast<AC_Sword>(Weapons[WeaponType])->SetCanCombo(true);
}

void AC_Unit::DisableCombo()
{
	Cast<AC_Sword>(Weapons[WeaponType])->SetCanCombo(false);
}

void AC_Unit::BeginCombo()
{
	Weapons[WeaponType]->BeginCombo();
}

void AC_Unit::ChangeWeapon(EWeaponState weaponState)
{
	if (GetCurrentWeapon() != nullptr)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetCurrentWeapon()->GetEquipMontage()))
			return;

		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(GetCurrentWeapon()->GetUnEquipMontage()))
			return;
	}

	switch (WeaponType)
	{
	case EWeaponState::UNARMED:

		WeaponType = weaponState;
		NextEquipWeapon = weaponState;

		if (weaponState == EWeaponState::SWORD)
		{
			if (EquipSwordSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EquipSwordSound, GetActorLocation(),0.2f);
			}
		}

		PlayAnimMontage(GetCurrentWeapon()->GetEquipMontage());
		break;
	default:
		PlayAnimMontage(GetCurrentWeapon()->GetUnEquipMontage());

		if (WeaponType != weaponState)
			NextEquipWeapon = weaponState;

		break;
	}

//	if (WeaponType == EWeaponState::SWORD)
		//StrafeOff();
	//else
		//StrafeOn();
}

void AC_Unit::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (bIsDead)
	{
		PlayAnimMontage(DeathMontage);

		SetActorEnableCollision(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		float time = 1.0f;

		if (DeathMontage)
			time = DeathMontage->GetPlayLength() * 0.9f;

		UKismetSystemLibrary::K2_SetTimer(this, "Terminate", time, false);
	}
}

void AC_Unit::ApplyStun(float Duration)
{
	if (CharacterState == ECharacterState::STUNNED)
	{
		return;
	}

	bIsStunned = true;

	CharacterState = ECharacterState::STUNNED;

	StunDuration = Duration;

	//StunStartTime = GetWorld()->GetTimeSeconds();
	//GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &AC_Unit::ResetStun, StunDuration, false);
}

void AC_Unit::ReduceStunTime()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	float TimeElapsedStun = CurrentTime - StunStartTime;

	//StunDuration -= 0.5f;
	StunDuration -= (0.5f + TimeElapsedStun);

	if (StunDuration <= 0)
	{
		ResetStun();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &AC_Unit::ResetStun, StunDuration, false);
		
		StunStartTime = CurrentTime;
	}
}

void AC_Unit::ResetStun()
{
	bCanStandUp = true;
}

void AC_Unit::CharacterStateNormal()
{
	bCanStandUp = false;
	bIsStunned = false;
	CharacterState = ECharacterState::NORMAL;
}

void AC_Unit::KnuckBack()
{
	FVector ForwardVector = GetActorForwardVector(); 
	FVector BackwardVector = -ForwardVector;

	FVector LaunchVelocity = BackwardVector * 600 + FVector(0, 0, 1) * 900; 
	LaunchCharacter(LaunchVelocity, true, true);
}

void AC_Unit::StartBlocking()
{
	if (CharacterState == ECharacterState::BLOCK)
	{
		return;
	}

	bIsBlocking = true;

	CharacterState = ECharacterState::BLOCK;
}

void AC_Unit::EndBlocking()
{
	bIsBlocking = false;
	CharacterState = ECharacterState::NORMAL;
}

void AC_Unit::ResetAttackDelay()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EnemyAttackDelay, this, &AC_Unit::SetAttackDelayFalse, 1.5f, false);

}

void AC_Unit::SetAttackDelayFalse()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "delayfalse");
	if (bIsAttackDelay == true)
	{
		bIsAttackDelay = false;
	}
	bCanMove = true;
}

float AC_Unit::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CurHp -= Damage;

	AC_Enemy* DamagedEnemy = Cast<AC_Enemy>(this);

	if (DamagedEnemy != nullptr)
	{
		direction = EventInstigator->GetPawn()->GetActorLocation() - this->GetActorLocation();

		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), EventInstigator->GetPawn()->GetActorLocation());

		rotator.Roll = 0;
		rotator.Pitch = 0;

		this->SetActorRotation(rotator);

		//this->SetActorLocation(this->GetActorLocation() - direction.GetSafeNormal2D() * 30.0f);

		BodyMaterial->SetVectorParameterValue("HitColor", FVector4(1, 0, 0, 1));

		GetMesh()->SetMaterial(0, BodyMaterial);

	}
	/*
	if (EventInstigator && EventInstigator->GetPawn())
	{
		FString InstigatorPawnName = EventInstigator->GetPawn()->GetName();
		UKismetSystemLibrary::PrintString(GetWorld(), InstigatorPawnName);
	}
	*/

	AC_Warrior* InstigatorWarrior = Cast<AC_Warrior>(EventInstigator->GetPawn());

	if (CurHp > 0.0f)
	{
		if(!bIsAttacking)
			PlayAnimMontage(HitMontage);
		bIsHit = true;

		UKismetSystemLibrary::K2_SetTimer(this, "ResetHit", 1.5f, false);

		if (InstigatorWarrior)
		{
			InstigatorWarrior->RecoverMP(2);
			PlayerLocation = InstigatorWarrior->GetActorLocation();
			//FString LocationString = PlayerLocation.ToString();
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Player Location: %s"), *LocationString));
		}

	}
	else 
	{
		if (!bIsDead&&!bIsFloating)
		{
			bIsDead = true;
			bIsHit = false;
			
			PlayAnimMontage(DeathMontage);

			if (InstigatorWarrior)
			{
				InstigatorWarrior->RecoverMP(5);
				InstigatorWarrior->AddExp(30);
			}

			SetActorEnableCollision(false);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			float time = 1.0f;

			if (DeathMontage)
				time = DeathMontage->GetPlayLength() * 0.9f;

			UKismetSystemLibrary::K2_SetTimer(this, "Terminate", time, false);
		}

		if (!bIsDead && bIsFloating)
		{
			bIsDead = true;

			if (InstigatorWarrior)
			{
				InstigatorWarrior->RecoverMP(5);
				InstigatorWarrior->AddExp(60);
			}

		}
		
	}

	return Damage;
	
}

void AC_Unit::Terminate()
{
	GetMesh()->bPauseAnims = true;

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	this->Destroy();

	for (auto weapon : Weapons)
	{
		weapon.Value->SetActorHiddenInGame(true);
		weapon.Value->Destroy();
	}
}

void AC_Unit::RestoreColor()
{
	BodyMaterial->SetVectorParameterValue("HitColor", FVector4(0, 0, 0, 1));

	GetMesh()->SetMaterial(0, BodyMaterial);
}

void AC_Unit::ResetHit()
{
	if(bIsHit)
		bIsHit = false;
}


