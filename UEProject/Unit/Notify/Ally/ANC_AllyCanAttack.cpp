// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Ally/ANC_AllyCanAttack.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"

FString UANC_AllyCanAttack::GetNotifyName_Implementation() const
{
	return "AllyCanAttack";
}

void UANC_AllyCanAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_AllyWarrior* ally = Cast<AC_AllyWarrior>(MeshComp->GetOwner());

	if (ally == nullptr)
		return;

	ally->SetCanAttack() = true;
}
