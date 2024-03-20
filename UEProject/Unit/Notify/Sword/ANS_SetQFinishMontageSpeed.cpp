// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Sword/ANS_SetQFinishMontageSpeed.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANS_SetQFinishMontageSpeed::GetNotifyName_Implementation() const
{
	return "SetQFinishMontageSpeed";
}

void UANS_SetQFinishMontageSpeed::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->SetQComboFinishMontageSpeed(0.7f);
}

void UANS_SetQFinishMontageSpeed::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->SetQComboFinishMontageSpeed(3.0f);
}
