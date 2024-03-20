// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Kick/ANS_KickActivateCollision.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANS_KickActivateCollision::GetNotifyName_Implementation() const
{
	return "KickActivateCollision";
}

void UANS_KickActivateCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->ActivateKickCollider();
}

void UANS_KickActivateCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->DeactivateKickCollider();
}
