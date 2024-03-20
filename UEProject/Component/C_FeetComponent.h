// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "C_FeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector LeftDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector RootBoneDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRotator RightRotation;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEPROJECT_API UC_FeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UC_FeetComponent();

protected:
	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);

	FFeetData GetData() { return Data; }

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float InterSpeed = 150.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float TraceDistance = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float OffsetDistance = 2.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName LeftSocket = "LeftFoot";

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName RightSocket = "RightFoot";

private:
	class AC_Unit* OwnerCharacter;

	FFeetData Data;

};
