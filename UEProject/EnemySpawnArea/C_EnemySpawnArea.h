// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_EnemySpawnArea.generated.h"

UCLASS()
class UEPROJECT_API AC_EnemySpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AC_EnemySpawnArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class AC_Enemy> EnemyClass;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ColliderBox;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

private:
	void CreateEnemies();
	void SpawnEnemy();

	//AController* OwnerController;

	TArray<class AC_Enemy*> Enemies;

	class AC_Enemy* LeaderEnemy = nullptr;

};
