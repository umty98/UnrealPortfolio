// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_BehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	WAIT,
	PATROL,
	TRACE,
	ATTACK,
	ATTACKNEAR,
	ATTACKMIDDLE,
	ATTACKFAR,
	HIT,
	DEATH
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEPROJECT_API UC_BehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UC_BehaviorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	bool IsCurrentType(EBehaviorType Type);

	EBehaviorType GetCurrentType();

	class AC_Unit* GetPlayer();
	class AC_Unit* GetEnemy();

	void ChangeType(EBehaviorType);

	void SetBlackBoard(class UBlackboardComponent* InBlackBoard) { this->BlackBoard = InBlackBoard; }

	FName GetPlayerKey() { return PlayerKey; }
	FName GetEnemyKey() { return EnemyKey; }

	void SetPlayer(class AActor* player);
	void SetEnemy(class AActor* enemy);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName BehaviorKey = "Behavior";

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName PlayerKey = "Player";

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName EnemyKey = "Enemy";

private:
	class UBlackboardComponent* BlackBoard;

};
