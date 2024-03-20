// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_Bow.h"
#include "Object/C_Arrow.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Unit/Warrior/C_Warrior.h"

void AC_Bow::BeginPlay()
{
	Super::BeginPlay();

	CreateArrows();
}

void AC_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_Bow::Attack()
{
	Super::Attack();
}

void AC_Bow::AppearArrow()
{
	for (auto arrow : Arrows)
	{
		if (arrow->GetIsAttached() || arrow->GetIsFired())
			continue;


		arrow->SetActorLocation(Owner->GetMesh()->GetSocketLocation("FiringSocket"));
		arrow->SetActorRotation(Owner->GetActorRotation());

		bIsAttachedInBow = true;
		arrow->AppearArrow();

		break;

	}
}

void AC_Bow::FireArrow()
{
	for (auto arrow : Arrows)
	{
		
		if (!arrow->GetIsAttached())
			continue;
		


		if (arrow->GetIsAttached())
		{
			arrow->SetOwner(this->warrior);
			arrow->Fire(warrior->GetBowAdditionalSpeed());
			bIsAttachedInBow = false;
		}
		
		break;
	}
	
}

UAnimMontage* AC_Bow::GetBowFireMontage() const
{
	if (SwordComboData.IsValidIndex(0))
	{
		return SwordComboData[0]->Animation;
	}
	return nullptr;
}

void AC_Bow::PlayFireArrow()
{
	Owner->PlayAnimMontage(SwordComboData[0]->Animation);
}

void AC_Bow::BeginCombo()
{
	
}

void AC_Bow::OnColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (Owner == otherActor)
		return;

	AC_Unit* unit = Cast<AC_Unit>(otherActor);

	if (unit == nullptr)
		return;

	if (Owner->GetGenericTeamId() == unit->GetGenericTeamId())
		return;

	if (!unit->CanBeHit())
		return;

	UGameplayStatics::ApplyDamage
	(
		otherActor,
		SwordComboData[0]->Damage,
		Owner->GetController(),
		this,
		nullptr
	);
	
	if (SwordComboData[0]->HitNiagara)
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			SwordComboData[0]->HitNiagara,
			overlappedComponent->GetComponentLocation(),
			overlappedComponent->GetComponentRotation()
		);

		NiagaraComponent->SetHiddenInSceneCapture(true);
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, this->GetActorLocation());
	}

	
	
}

void AC_Bow::CreateArrows()
{
	FActorSpawnParameters param;
	param.Owner = this;

	for (int32 i = 0; i < 10; i++)
	{
		AC_Arrow* arrow = GetWorld()->SpawnActor<AC_Arrow>(ArrowClass, param);

		//arrow->SetActive(false);
		arrow->DeactiveArrow();

		Arrows.Add(arrow);
	}
}
