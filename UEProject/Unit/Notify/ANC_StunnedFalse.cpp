// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/ANC_StunnedFalse.h"
#include "Unit/C_Unit.h"

FString UANC_StunnedFalse::GetNotifyName_Implementation() const
{
	return "StunnedFalse";
}

void UANC_StunnedFalse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;


	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	unit->CharacterStateNormal();
}
