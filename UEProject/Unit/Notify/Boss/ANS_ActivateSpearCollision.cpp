// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Boss/ANS_ActivateSpearCollision.h"
#include "Unit/Boss/C_Boss.h"

FString UANS_ActivateSpearCollision::GetNotifyName_Implementation() const
{
	return "ActivateSpearCollision";
}

void UANS_ActivateSpearCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Boss* boss = Cast<AC_Boss>(MeshComp->GetOwner());

	if (boss == nullptr)
		return;

	boss->ActivateSpearCollision();
}

void UANS_ActivateSpearCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Boss* boss = Cast<AC_Boss>(MeshComp->GetOwner());

	if (boss == nullptr)
		return;

	boss->DeactivateSpearCollision();
}
