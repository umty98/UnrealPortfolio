// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_ESkillColliderBox.h"
#include "Unit/Enemy/C_Enemy.h"
#include "Unit/C_Unit.h"

// Sets default values
AC_ESkillColliderBox::AC_ESkillColliderBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

void AC_ESkillColliderBox::ActivateCollision()
{
	if (BoxCollider)
	{
		BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AC_ESkillColliderBox::DeacivateCollision()
{
	if (BoxCollider)
	{
		BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Victims.Empty();
	}
}

void AC_ESkillColliderBox::OnESkillOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AC_Unit* unit = Cast<AC_Unit>(OtherActor);
	
	if (Owner == OtherActor)
		return;
	
	if (unit == nullptr)
		return;

	if (Victims.Find(unit) >= 0)
		return;

	Victims.Add(unit);

	if (bCanLaunchByESkill)
		ESkillDamage = 10.0f;
	else
		ESkillDamage = 5.0f;

	if (unit && unit->CanBeHit())
	{
		if (bCanLaunchByESkill)
		{
			AC_Enemy* DEnemy = Cast<AC_Enemy>(unit);
			if(DEnemy!=nullptr)
				DEnemy->LaunchIntoAirESkill();
		}

		UGameplayStatics::ApplyDamage(
			OtherActor,
			ESkillDamage,
			GetOwnerController(),
			this,
			UDamageType::StaticClass()
		);
	}
	


}

// Called when the game starts or when spawned
void AC_ESkillColliderBox::BeginPlay()
{
	Super::BeginPlay();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AC_ESkillColliderBox::OnESkillOverlap);
	
}

// Called every frame
void AC_ESkillColliderBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


