// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Boss/C_BossNearShield.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Unit/Warrior/C_Warrior.h"
#include "Unit/C_Unit.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AC_BossNearShield::AC_BossNearShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShieldCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollider"));
	RootComponent = ShieldCollider;

	ShieldCollider->InitSphereRadius(100.0f);
	ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldCollider->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));

	ShieldActiveComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldActiveComponent"));
	ShieldActiveComponent->SetupAttachment(RootComponent);
	ShieldActiveComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

	ShieldExplosiveComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldExplosiveComponent"));
	ShieldExplosiveComponent->SetupAttachment(RootComponent);
	ShieldExplosiveComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
}

// Called when the game starts or when spawned
void AC_BossNearShield::BeginPlay()
{
	Super::BeginPlay();

	ShieldCollider->OnComponentBeginOverlap.AddDynamic(this, &AC_BossNearShield::OnBodyColliderBeginOverlap);

	if (ShieldActiveSystem)
	{
		ShieldActiveComponent->SetAsset(ShieldActiveSystem);
		ShieldActiveComponent->Deactivate();
	}

	if (ShieldExplosiveSystem)
	{
		ShieldExplosiveComponent->SetAsset(ShieldExplosiveSystem);
		ShieldExplosiveComponent->Deactivate();
	}

	
}

void AC_BossNearShield::OnBodyColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (Owner == otherActor)
		return;

	AC_Unit* unit = Cast<AC_Unit>(otherActor);

	if (unit == nullptr)
		return;

	if (Victims.Find(unit) >= 0)
		return;

	Victims.Add(unit);

	UGameplayStatics::ApplyDamage(
		otherActor,
		50.0f,
		GetOwnerController(),
		this,
		UDamageType::StaticClass()
	);

	
	AC_Warrior* Dwarrior = Cast<AC_Warrior>(unit);
	if (Dwarrior != nullptr)
	{
		Dwarrior->ApplyStun(5.0f);
		Dwarrior->KnuckBack();
		Dwarrior->StunnedBarOn();
	}
	
}

// Called every frame
void AC_BossNearShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_BossNearShield::ActivateCollision()
{
	if (ShieldCollider)
	{
		ShieldCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AC_BossNearShield::DeactivateCollision()
{
	if (ShieldCollider)
	{
		ShieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Victims.Empty();
	}
}

