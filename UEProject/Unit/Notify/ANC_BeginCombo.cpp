// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/ANC_BeginCombo.h"
#include "ANC_BeginCombo.h"
#include "Unit/Knight/C_Knight.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANC_BeginCombo::GetNotifyName_Implementation() const
{
	return "BeginCombo";
}

void UANC_BeginCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;


	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	unit->BeginCombo();
}
