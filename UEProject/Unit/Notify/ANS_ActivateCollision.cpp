// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/ANS_ActivateCollision.h"
#include "Unit/C_Unit.h"
#include "Object/C_Weapon.h"
#include "Object/C_Sword.h"

FString UANS_ActivateCollision::GetNotifyName_Implementation() const
{
	return "ActivateCollision";
}

void UANS_ActivateCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	Cast<AC_Sword>(unit->GetCurrentWeapon())->ActivateCollision();
}

void UANS_ActivateCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	Cast<AC_Sword>(unit->GetCurrentWeapon())->DeactivateCollision();
}
