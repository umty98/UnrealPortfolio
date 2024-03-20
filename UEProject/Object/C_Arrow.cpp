// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ShapeComponent.h"
#include "Unit/C_Unit.h"
#include "Unit/Warrior/C_Warrior.h"
#include "Object/C_Bow.h"

// Sets default values
AC_Arrow::AC_Arrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	//ProjectileMovement->ProjectileGravityScale = 1.0f;
}

// Called when the game starts or when spawned
void AC_Arrow::BeginPlay()
{
	Super::BeginPlay();

	AC_Bow* bow = Cast<AC_Bow>(GetOwner());

	Collider = Cast<UShapeComponent>(GetComponentByClass(UShapeComponent::StaticClass()));
	Collider->OnComponentBeginOverlap.AddDynamic(bow, &AC_Bow::OnColliderBeginOverlap);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AC_Arrow::OnColliderBeginOverlap);

	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AC_Arrow::OnColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == Owner)
		return;

	//AC_Bow* bow = Cast<AC_Bow>(Owner);

	//if (bow->GetOwner() == otherActor)
		//return;

	DeactiveArrow();
}


// Called every frame
void AC_Arrow::Tick(float DeltaTime)
{
	if (!bIsAttached)
		return;

	if (!bIsAttached && !bIsFired)
		return;


	Super::Tick(DeltaTime);

	if (bIsAttached)
	{
		AC_Bow* bow = Cast<AC_Bow>(GetOwner());
		if (bow)
		{
			AC_Warrior* ownerWarrior = Cast<AC_Warrior>(bow->GetOwner());

			if (ownerWarrior)
			{
				ownerWarrior->RotateTowardsRaycastHitForZoom();
				FVector socketLocation = ownerWarrior->GetMesh()->GetSocketLocation("FiringSocket");
				FRotator socketRotation = ownerWarrior->GetMesh()->GetSocketRotation("FiringSocket");

				SetActorLocation(socketLocation);
				SetActorRotation(socketRotation);
			}
		}
	}
}

void AC_Arrow::SetActive(bool IsActive)
{
	bIsActive = IsActive;

	/*
	if (bIsActive)
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		ProjectileMovement->Velocity = GetActorForwardVector() * Speed;
		ProjectileMovement->bSimulationEnabled = true;
	}
	else
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);

		ProjectileMovement->Velocity = FVector::ZeroVector;
		ProjectileMovement->bSimulationEnabled = false;
	}
	*/
}

void AC_Arrow::SetDirection(FVector Direction)
{
	ProjectileMovement->Velocity = Direction * Speed;
}

void AC_Arrow::AppearArrow()
{
	SetActorHiddenInGame(false);
	bIsAttached = true;
}

void AC_Arrow::Fire(float PlusSpeed)
{
	/*
	bIsFired = true;
	bIsAttached = false;
	SetActorEnableCollision(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMovement->Velocity = GetActorForwardVector() * Speed;
	ProjectileMovement->bSimulationEnabled = true;
	*/

	FVector HitLocation;

	float FinalSpeed = Speed + PlusSpeed;

	if (Owner && Owner->PerformRaycastFromCamera(HitLocation))
	{
		FVector FireDirection = (HitLocation - GetActorLocation()).GetSafeNormal();
		FireDirection.Z += 0.2f;
		ProjectileMovement->Velocity = FireDirection * FinalSpeed;
	}
	else
	{
		ProjectileMovement->Velocity = GetActorForwardVector() * FinalSpeed;
	}

	bIsFired = true;
	bIsAttached = false;
	SetActorEnableCollision(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMovement->bSimulationEnabled = true;
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AC_Arrow::DeactivateArrowIfNotHit, 5.0f, false);
}

void AC_Arrow::DeactiveArrow()
{

	bIsFired = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->bSimulationEnabled = false;

	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}


void AC_Arrow::DeactivateArrowIfNotHit()
{
	if (bIsFired)
	{
		DeactiveArrow();
	}
}



