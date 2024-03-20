// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_Sword.h"
#include "Components/ShapeComponent.h"
#include "Unit/Warrior/C_Warrior.h"
#include "Unit/C_Unit.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h" 
#include "Sound/SoundBase.h"

void AC_Sword::BeginPlay()
{
	Super::BeginPlay();

	BodyCollider = Cast<UShapeComponent>(GetComponentByClass(UShapeComponent::StaticClass()));
	BodyCollider->OnComponentBeginOverlap.AddDynamic(this, &AC_Sword::OnBodyColliderBeginOverlap);
	BodyCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AC_Sword::Attack()
{
	//if (bBeginCombo)
	//	return;

	if (bCanCombo)
	{
		bBeginCombo = true;
		return;
	}

	if (Owner->GetIsAttacking())
		return;

	//if (Owner->GetMesh()->GetAnimInstance()->Montage_IsPlaying(SwordComboData[0]->Animation))
	//	return;

	//Owner->PlayAnimMontage(SwordComboData[0]->Animation);

	Super::Attack();

	ComboCount = 1;
}

void AC_Sword::BeginCombo()
{
	if (bBeginCombo == false)
		return;

	if (!SwordComboData.IsValidIndex(ComboCount))
		ComboCount = 0;


	Owner->PlayAnimMontage(SwordComboData[ComboCount]->Animation);
	Owner->SetCanMove(SwordComboData[ComboCount++]->bCanMove);

	bBeginCombo = false;
}

void AC_Sword::ActivateCollision()
{
	BodyCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AC_Sword::DeactivateCollision()
{
	BodyCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Victims.Empty();
}

void AC_Sword::QSkillComboFinish()
{
	Owner->SetCanMove(SwordComboData[3]->bCanMove);
	Owner->PlayAnimMontage(SwordComboData[3]->Animation,3.0f);

	ComboCount = 4;
}

void AC_Sword::QSkillComboStart()
{
	Owner->SetCanMove(SwordComboData[4]->bCanMove);
	Owner->PlayAnimMontage(SwordComboData[4]->Animation,3.0f);

	ComboCount = 5;
}

void AC_Sword::ESkillComboStart()
{
	Owner->SetCanMove(SwordComboData[5]->bCanMove);
	Owner->PlayAnimMontage(SwordComboData[5]->Animation,2.0f);

	ComboCount = 6;
}

void AC_Sword::ESkillComboFinish()
{
	Owner->SetCanMove(SwordComboData[6]->bCanMove);
	Owner->PlayAnimMontage(SwordComboData[6]->Animation);

	ComboCount = 7;
}

UAnimMontage* AC_Sword::GetQSkillComboFinishMontage() const
{
	if (SwordComboData.IsValidIndex(3))
	{
		return SwordComboData[3]->Animation;
	}
	return nullptr;
}

UAnimMontage* AC_Sword::GetESkillStartMontage() const
{
	if (SwordComboData.IsValidIndex(3))
	{
		return SwordComboData[5]->Animation;
	}
	return nullptr;
}

void AC_Sword::OnBodyColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (Owner == otherActor)
		return;

	AC_Unit* unit = Cast<AC_Unit>(otherActor);

	if (unit == nullptr)
		return;

	if (Owner->GetGenericTeamId() == unit->GetGenericTeamId())
		return;

	AC_Warrior* Dwarrior = Cast<AC_Warrior>(unit);
	AC_Enemy* Denemy = Cast<AC_Enemy>(Owner);
	if (Dwarrior != nullptr && Denemy != nullptr && Dwarrior->GetIsBlocking())
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "reactmontage");
		Denemy->PlayReactMontage();
		Dwarrior->RecoverMP(5.0f);
		Dwarrior->SucceedBlockCameraShake();
		return;
	}

	if (Victims.Find(unit) >= 0)
		return;

	Victims.Add(unit);

	if (!unit->CanBeHit())
		return;

	/*
	AC_Warrior* Dwarrior = Cast<AC_Warrior>(unit);
	AC_Enemy* Denemy = Cast<AC_Enemy>(Owner);
	if (Dwarrior != nullptr && Denemy != nullptr)
	{
		Dwarrior->ApplyStun(5.0f);
	}
	*/

	


	UGameplayStatics::ApplyDamage
	(
		otherActor,
		SwordComboData[ComboCount - 1]->Damage,
		Owner->GetController(),
		this,
		nullptr
	);

	/*
	if (SwordComboData[ComboCount - 1]->Effect)
	{
		UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			SwordComboData[ComboCount - 1]->Effect,
			this->GetTransform()
		);

		if (ParticleSystemComponent)
		{
			ParticleSystemComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f)); // NewScale은 FVector 타입이며, 원하는 스케일 값으로 설정해야 함.
		}
	}
	*/

	if (SwordComboData[ComboCount - 1]->HitNiagara)
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SwordComboData[ComboCount - 1]->HitNiagara,
			overlappedComponent->GetComponentLocation(),
			overlappedComponent->GetComponentRotation()
		);

		NiagaraComponent->SetHiddenInSceneCapture(true);

		if (NiagaraComponent)
		{
			NiagaraComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f)); 
		}
	}


	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, this->GetActorLocation(),0.05f);
	}
}
