// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Boss/C_BossFallingSpear.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Unit/C_Unit.h"


// Sets default values
AC_BossFallingSpear::AC_BossFallingSpear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetWorldScale3D(FVector(4.0f,4.0f,4.0f));

	FallingSpearComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FallingSpearComponent"));
	FallingSpearComponent->SetupAttachment(RootComponent);
	FallingSpearComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	FallingSpearComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	FallingSpearComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));


	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);

	DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	DecalComponent->DecalSize = FVector(100.0f, 100.0f, 100.0f);
	DecalComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -17.0f));
	DecalComponent->SetRelativeScale3D(FVector(0.15f, 0.4f, 0.4f));


}

// Called when the game starts or when spawned
void AC_BossFallingSpear::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AC_BossFallingSpear::OnSpearColliderBeginOverlap);

	if (FallingSpearSystem)
	{
		FallingSpearComponent->SetAsset(FallingSpearSystem);
		FallingSpearComponent->Deactivate();
	}
	
}

void AC_BossFallingSpear::OnSpearColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
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
		15.0f,
		GetOwnerController(),
		this,
		UDamageType::StaticClass()
	);
}

// Called every frame
void AC_BossFallingSpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_BossFallingSpear::ActivateNiagara()
{
	FallingSpearComponent->Activate(true);
}

void AC_BossFallingSpear::ActivateCollision()
{
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AC_BossFallingSpear::DeactivateCollision()
{
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Victims.Empty();
	}
}

