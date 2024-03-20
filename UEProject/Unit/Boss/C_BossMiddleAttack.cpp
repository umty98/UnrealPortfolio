// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Boss/C_BossMiddleAttack.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Unit/C_Unit.h"
#include "Unit/Warrior/C_Warrior.h"

// Sets default values
AC_BossMiddleAttack::AC_BossMiddleAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetWorldScale3D(FVector(47.0f, 47.0f, 47.0f));

	MiddleAttackComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MiddleAttackComponent"));
	MiddleAttackComponent->SetupAttachment(RootComponent);
	MiddleAttackComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -3.0f));
	MiddleAttackComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);

	DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	DecalComponent->DecalSize = FVector(100.0f, 100.0f, 100.0f);
	DecalComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
	DecalComponent->SetRelativeScale3D(FVector(0.7f, 0.35f, 0.35f));

}

// Called when the game starts or when spawned
void AC_BossMiddleAttack::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AC_BossMiddleAttack::OnMiddleColliderBeginOverlap);

	if (MiddleAttackSystem)
	{
		MiddleAttackComponent->SetAsset(MiddleAttackSystem);
		MiddleAttackComponent->Deactivate();
	}
	
}

void AC_BossMiddleAttack::OnMiddleColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
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
		30.0f,
		GetOwnerController(),
		this,
		UDamageType::StaticClass()
	);


	AC_Warrior* Dwarrior = Cast<AC_Warrior>(unit);
	if (Dwarrior != nullptr)
	{
		Dwarrior->Freezed();
	}



}

// Called every frame
void AC_BossMiddleAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_BossMiddleAttack::ActivateNiagara()
{
	MiddleAttackComponent->Activate(true);
	DecalComponent->SetVisibility(false);
}

void AC_BossMiddleAttack::ActivateCollision()
{
	ActivateNiagara();
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AC_BossMiddleAttack::DeactivateCollision()
{
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Victims.Empty();
	}
}

