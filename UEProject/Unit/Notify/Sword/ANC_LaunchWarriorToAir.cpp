// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Notify/Sword/ANC_LaunchWarriorToAir.h"
#include "Unit/Warrior/C_Warrior.h"

FString UANC_LaunchWarriorToAir::GetNotifyName_Implementation() const
{
	return "LaunchWarriorToAir";
}

void UANC_LaunchWarriorToAir::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr)
		return;

	if (MeshComp->GetOwner() == nullptr)
		return;

	class AC_Warrior* warrior = Cast<AC_Warrior>(MeshComp->GetOwner());


	if (warrior == nullptr)
		return;

	warrior->LaunchWarriorToAir();
}
