// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_FallingArrow.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Unit/Enemy/C_Enemy.h"

// Sets default values
AC_FallingArrow::AC_FallingArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	// Create a Niagara component for the visual effect
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AC_FallingArrow::ActivateCollision()
{
	if (CapsuleComponent)
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AC_FallingArrow::DeactivateCollision()
{
	if (CapsuleComponent)
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Victims.Empty();
	}
}

// Called when the game starts or when spawned
void AC_FallingArrow::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AC_FallingArrow::OnBodyColliderBeginOverlap);
	
	if (ArrowNiagaraSystem)
	{
		NiagaraComponent->SetAsset(ArrowNiagaraSystem);
	}
	
	// Setup collision responses or any other initial settings for the capsule
	//CapsuleComponent->OnComponentHit.AddDynamic(this, &AC_FallingArrow::OnHit); // Register a hit event
}

void AC_FallingArrow::OnBodyColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	AC_Enemy* HitEnemy = Cast<AC_Enemy>(otherActor);

	if (HitEnemy == nullptr)
		return;

	if (Victims.Find(HitEnemy) >= 0)
		return;

	Victims.Add(HitEnemy);

	if (HitEnemy && HitEnemy->CanBeHit())
	{
		UGameplayStatics::ApplyDamage(
			HitEnemy,
			5.0f,
			GetOwnerController(),
			this,
			UDamageType::StaticClass()
		);
	}
}

// Called every frame
void AC_FallingArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFalling)
		DropArrow();

}

void AC_FallingArrow::DropArrow()
{
	FVector Velocity = FVector(0, 0, -1380); 
	SetActorLocation(GetActorLocation() + Velocity * GetWorld()->GetDeltaSeconds());
}


void AC_FallingArrow::SetIsFallingTrue()
{
	bIsFalling = true;
}

