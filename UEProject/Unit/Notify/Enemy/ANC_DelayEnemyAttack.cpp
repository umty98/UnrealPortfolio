// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Enemy/ANC_DelayEnemyAttack.h"
#include "Unit/C_Unit.h"

FString UANC_DelayEnemyAttack::GetNotifyName_Implementation() const
{
    return "DelayAttack";
}

void UANC_DelayEnemyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Unit* unit = Cast<AC_Unit>(MeshComp->GetOwner());

	if (unit == nullptr)
		return;

	unit->ResetAttackDelay();
}
