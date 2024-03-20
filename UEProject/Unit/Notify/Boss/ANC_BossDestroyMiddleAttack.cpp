// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Boss/ANC_BossDestroyMiddleAttack.h"
#include "Unit/Boss/C_Boss.h"

FString UANC_BossDestroyMiddleAttack::GetNotifyName_Implementation() const
{
	return "BossDestroyMiddleAttack";
}

void UANC_BossDestroyMiddleAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Boss* boss = Cast<AC_Boss>(MeshComp->GetOwner());

	if (boss == nullptr)
		return;

	boss->DestroyMiddleAttack();
}
