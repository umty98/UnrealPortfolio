// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAI/C_BehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UC_BehaviorComponent::UC_BehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_BehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UC_BehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UC_BehaviorComponent::IsCurrentType(EBehaviorType Type)
{
	return GetCurrentType() == Type;
}

EBehaviorType UC_BehaviorComponent::GetCurrentType()
{
	return EBehaviorType(BlackBoard->GetValueAsEnum(BehaviorKey));
}

AC_Unit* UC_BehaviorComponent::GetPlayer()
{
	return Cast<AC_Unit>(BlackBoard->GetValueAsObject(PlayerKey));
}

AC_Unit* UC_BehaviorComponent::GetEnemy()
{
	return Cast<AC_Unit>(BlackBoard->GetValueAsObject(EnemyKey));
}

void UC_BehaviorComponent::ChangeType(EBehaviorType Type)
{
	BlackBoard->SetValueAsEnum(BehaviorKey, (uint8)Type);
}

void UC_BehaviorComponent::SetPlayer(AActor* player)
{
	BlackBoard->SetValueAsObject(PlayerKey, player);
}

void UC_BehaviorComponent::SetEnemy(AActor* enemy)
{
	BlackBoard->SetValueAsObject(EnemyKey, enemy);
}

