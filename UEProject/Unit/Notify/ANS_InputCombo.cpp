// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/ANS_InputCombo.h"
#include "Unit/Knight/C_Knight.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANS_InputCombo::GetNotifyName_Implementation() const
{
	return "InputCombo";
}

void UANS_InputCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	unit->EnableCombo();
}

void UANS_InputCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;


	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	unit->DisableCombo();
}
