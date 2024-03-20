// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/C_BossPortal.h"
#include "Components/BoxComponent.h"
#include "Unit/C_Unit.h"
#include "Unit/Warrior/C_Warrior.h"
#include "EngineUtils.h"
#include "Particles/ParticleSystemComponent.h" 
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"

// Sets default values
AC_BossPortal::AC_BossPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderBox"));
	RootComponent = ColliderBox;

	PortalDefenceComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PortalDefenceComponent"));
	PortalDefenceComponent->SetupAttachment(RootComponent);
	PortalDefenceComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -48.0f));
	PortalDefenceComponent->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
	SphereComponent->SetRelativeScale3D(FVector(2.3f, 2.3f, 2.3f));
	SphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
}

// Called when the game starts or when spawned
void AC_BossPortal::BeginPlay()
{
	Super::BeginPlay();
	
	ColliderBox->OnComponentBeginOverlap.AddDynamic(this, &AC_BossPortal::OnComponentColliderBeginOverlap);

	if (PortalDefenceSystem)
	{
		PortalDefenceComponent->SetAsset(PortalDefenceSystem);
	}
}

void AC_BossPortal::OnComponentColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	AC_Unit* unit = Cast<AC_Unit>(otherActor);

	if (unit == nullptr)
		return;

	AC_Warrior* Dwarrior = Cast<AC_Warrior>(unit);
	if (Dwarrior == nullptr)
		return;

	Dwarrior->BossMapSetting();
	Dwarrior->SavePlayerInstance();
}

void AC_BossPortal::CheckForEnemiesAndActivatePortal()
{

	bool bEnemyPresent = false;

	for (TActorIterator<AC_Enemy> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		bEnemyPresent = true;
		break;  
	}

	
	if (!bEnemyPresent)
	{
		bIsPortalCanActive = true;
		//ActivatePortal();
		FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, -150.0f);
		PortalActiveComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PortalActiveSystem, SpawnLocation, GetActorRotation(), true);

		FVector NewScale(3.0f, 3.0f, 5.0f);
		PortalActiveComponent->SetWorldScale3D(NewScale);
		PortalDefenceComponent->Deactivate();
		//PortalDefenceComponent->SetHiddenInGame(true);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	else
	{
		bIsPortalCanActive = false;
		//DeactivatePortal();
	}
	
}

// Called every frame
void AC_BossPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsPortalCanActive)
	{
		AccumulatedTime += DeltaTime;
		if (AccumulatedTime >= CheckInterval)
		{
			CheckForEnemiesAndActivatePortal();
			AccumulatedTime = 0.0f;
		}
	}

	
}

