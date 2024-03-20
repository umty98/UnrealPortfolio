// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Sword/ANC_LandAfterQComboFinish.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANC_LandAfterQComboFinish::GetNotifyName_Implementation() const
{
	return "LandAfterQComboFinish";
}

void UANC_LandAfterQComboFinish::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());

	if (warrior == nullptr)
		return;

	warrior->AfterQComboFinish();
}
