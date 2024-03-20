// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Sword/ANS_QSKillActivateCollision.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANS_QSKillActivateCollision::GetNotifyName_Implementation() const
{
	return "QSKillActivateCollision";
}

void UANS_QSKillActivateCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->ActivateQSkillCollider();
}

void UANS_QSKillActivateCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->DeactiveQSkillCollider();
}
