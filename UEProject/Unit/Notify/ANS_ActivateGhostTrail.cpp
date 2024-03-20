// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/ANS_ActivateGhostTrail.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANS_ActivateGhostTrail::GetNotifyName_Implementation() const
{
	return "ActivateGhostTrail";
}

void UANS_ActivateGhostTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->ActivateGhostTrail();

}

void UANS_ActivateGhostTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->DeactivateGhostTrail();
}
