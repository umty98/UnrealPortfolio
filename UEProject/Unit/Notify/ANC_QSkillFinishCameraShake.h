// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ANC_QSkillFinishCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UANC_QSkillFinishCameraShake : public UAnimNotify
{
	GENERATED_BODY()

	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
