// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/ANC_UnequipSword.h"
#include "Unit/Knight/C_Knight.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANC_UnequipSword::GetNotifyName_Implementation() const
{
	return "UnequipSword";
}

void UANC_UnequipSword::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;


	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	unit->UnEquipWeapon();
}
