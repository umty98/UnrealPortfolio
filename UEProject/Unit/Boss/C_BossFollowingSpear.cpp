// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Boss/C_BossFollowingSpear.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Unit/C_Unit.h"
#include "Unit/Warrior/C_Warrior.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AC_BossFollowingSpear::AC_BossFollowingSpear()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));

	FollowingArrowComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FollowingArrowComponent"));
	FollowingArrowComponent->SetupAttachment(SphereComponent);
	FollowingArrowComponent->SetRelativeLocation(FVector(-26.6667f, 0.0f, 0.0f));

	FollowingArrowSpawnComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FollowingArrowSpawnComponent"));
	FollowingArrowSpawnComponent->SetupAttachment(SphereComponent);
	FollowingArrowSpawnComponent->SetRelativeLocation(FVector(-146.6667f, 0.0f, 0.0f));
	FollowingArrowSpawnComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	FollowingArrowExplosiveComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FollowingArrowExplosiveComponent"));
	FollowingArrowExplosiveComponent->SetupAttachment(SphereComponent);
	FollowingArrowExplosiveComponent->SetRelativeLocation(FVector(3.33f, 0.0f, -43.33f));


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

}

// Called when the game starts or when spawned
void AC_BossFollowingSpear::BeginPlay()
{
	Super::BeginPlay();

	if (FollowingArrowSystem)
	{
		FollowingArrowComponent->SetAsset(FollowingArrowSystem);
	}
	FollowingArrowComponent->Deactivate();

	if (FollowingArrowSpawnSystem)
	{
		FollowingArrowSpawnComponent->SetAsset(FollowingArrowSpawnSystem);
	}

	if (FollowingArrowExplosiveSystem)
	{
		FollowingArrowExplosiveComponent->SetAsset(FollowingArrowExplosiveSystem);
	}
	FollowingArrowExplosiveComponent->Deactivate();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AC_BossFollowingSpear::OnSpearColliderBeginOverlap);

	//FollowingArrowComponent->Activate(true);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AC_BossFollowingSpear::SpawnSpear, 1.5f, false);
	
}

void AC_BossFollowingSpear::OnSpearColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
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
		20.0f,
		GetOwnerController(),
		this,
		UDamageType::StaticClass()
	);

	DestorySpear();
}

// Called every frame
void AC_BossFollowingSpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFired)
	{
		AC_Warrior* PlayerWarrior = Cast<AC_Warrior>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (PlayerWarrior)
		{
			ProjectileMovement->HomingTargetComponent = PlayerWarrior->GetRootComponent();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerWarrior->GetActorLocation());
			SetActorRotation(LookAtRotation);
		}
	}



}

void AC_BossFollowingSpear::FireSpear()
{
	bIsFired = true;

	AC_Warrior* PlayerWarrior = Cast<AC_Warrior>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerWarrior)
	{
		ProjectileMovement->HomingTargetComponent = PlayerWarrior->GetRootComponent();
	}
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 800.0f;
	ProjectileMovement->MaxSpeed = 700.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	ActivateCollision();

	GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &AC_BossFollowingSpear::SpearExplosive, 10.0f, false);
}

void AC_BossFollowingSpear::SpawnSpear()
{
	FollowingArrowSpawnComponent->Deactivate();
	FollowingArrowComponent->Activate(true);

	FireSpear();
}

void AC_BossFollowingSpear::DestorySpear()
{
	bIsFired = false;
	bCanExplode = false;
	//UKismetSystemLibrary::PrintString(GetWorld(), "DestorySpear");
	//FollowingArrowComponent->Deactivate();
	DeactivateCollision();
	Destroy();
}

void AC_BossFollowingSpear::SpearExplosive()
{
	bCanExplode = true;
	bIsFired = false;
	FollowingArrowComponent->SetHiddenInGame(true);
	FollowingArrowExplosiveComponent->Activate(true);

	ProjectileMovement->MaxSpeed = 0.0f;

	//UKismetSystemLibrary::PrintString(GetWorld(), "SpearExplosive");
	GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &AC_BossFollowingSpear::DestorySpearIfNotHit, 2.0f, false);
}

void AC_BossFollowingSpear::DestorySpearIfNotHit()
{
	if (bCanExplode)
	{
		//UKismetSystemLibrary::PrintString(GetWorld(), "DestorySpearIfNotHit");
		Destroy();
	}

}

void AC_BossFollowingSpear::ActivateCollision()
{
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

}

void AC_BossFollowingSpear::DeactivateCollision()
{
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Victims.Empty();
	}
}

