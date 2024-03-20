// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_AllySpawn.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Unit/Warrior/C_Warrior.h"
#include "AI/AllyAI/C_AllyAIController.h"
#include "Unit/AllyWarrior/C_AllyWarrior.h"
#include "Particles/ParticleSystemComponent.h" 
#include "EngineUtils.h"

// Sets default values
AC_AllySpawn::AC_AllySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColliderBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = ColliderBox; // RootComponent에 부착
	ColliderBox->SetBoxExtent(FVector(30.0f, 30.0f, 50.0f)); // 크기 설정
	ColliderBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);

	DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	DecalComponent->DecalSize = FVector(100.0f, 100.0f, 100.0f);




}

// Called when the game starts or when spawned
void AC_AllySpawn::BeginPlay()
{
	Super::BeginPlay();
	
	ColliderBox->OnComponentBeginOverlap.AddDynamic(this, &AC_AllySpawn::OnComponentColliderBeginOverlap);

	//DecalComponent->SetHiddenInGame(true);
}

// Called every frame
void AC_AllySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if(!bIsWarriorNear)
		//CheckAndPlayParticleSystem();
}

void AC_AllySpawn::CheckAndPlayParticleSystem()
{
	bool bFoundWarrior = false;

	// 모든 AC_Warrior 인스턴스를 검사
	for (TActorIterator<AC_Warrior> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AC_Warrior* warrior = *ActorItr;
		if (warrior)
		{
			float Distance = FVector::Dist(this->GetActorLocation(), warrior->GetActorLocation());
			if (Distance < 1000.0f)
			{
				bIsWarriorNear = true;
				bFoundWarrior = true;
				break;
			}
		}
	}

	UpdateParticleSystem(bFoundWarrior);
}

void AC_AllySpawn::UpdateParticleSystem(bool bWarriorNear)
{
	if (bWarriorNear)
	{
		// Warrior가 200.0f 이내에 있으면 파티클 컴포넌트가 있을 경우 제거하거나 숨김
		if (SpawnLocationComponent)
		{
			SpawnLocationComponent->SetVisibility(false);
			SpawnLocationComponent->DeactivateSystem();
		}
		bIsWarriorNear = true;
	}
	else
	{
		if (!SpawnLocationComponent)
		{
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.Z -= 450.0f;
			SpawnLocationComponent = UGameplayStatics::SpawnEmitterAttached(SpawnLocationSystem, RootComponent, NAME_None, SpawnLocation, GetActorRotation(), EAttachLocation::KeepWorldPosition, true);
			FVector NewScale = FVector(0.5f, 0.5f, 3.0f); 
			//SpawnLocationComponent->SetRelativeLocation(NewLocation);
			SpawnLocationComponent->SetWorldScale3D(NewScale);
			DecalComponent->SetHiddenInGame(false);
		}
		else
		{
			SpawnLocationComponent->SetVisibility(true);
			SpawnLocationComponent->ActivateSystem();
		}

		bIsWarriorNear = false;
	}
}

void AC_AllySpawn::OnComponentColliderBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	Warrior = Cast<AC_Warrior>(otherActor);

	if (Warrior == nullptr)
		return;

	if (Warrior)
	{
		PlayerRotation = Warrior->GetActorRotation();
		SpawnAlly();
		ColliderBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Warrior->ActivateSpawnCamera();
	}


}

void AC_AllySpawn::SpawnAlly()
{
	FVector Origin = GetActorLocation();
	FVector ForwardVector = PlayerRotation.Vector();

	SpawnPositions.Add(Origin + (ForwardVector.RotateAngleAxis( 90, FVector(0, 0, 1)) * 200));
	SpawnPositions.Add(Origin + (ForwardVector.RotateAngleAxis(-90, FVector(0, 0, 1)) * 200));
	SpawnPositions.Add(Origin + (ForwardVector.RotateAngleAxis( 90, FVector(0, 0, 1)) * 400));
	SpawnPositions.Add(Origin + (ForwardVector.RotateAngleAxis(-90, FVector(0, 0, 1)) * 400));
	SpawnPositions.Add(Origin + (ForwardVector.RotateAngleAxis(90,  FVector(0, 0, 1)) * 600));
	SpawnPositions.Add(Origin + (ForwardVector.RotateAngleAxis(-90, FVector(0, 0, 1)) * 600));

	CurrentSpawnIndex = 0;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AC_AllySpawn::SpawnNextAlly, 1.0f, true, 0.0f);
}

void AC_AllySpawn::SpawnNextAlly()
{
	if (CurrentSpawnIndex < SpawnPositions.Num())
	{
		FVector SpawnLocation = SpawnPositions[CurrentSpawnIndex];
		SpawnLocation.Z += 50.0f;
		FRotator SpawnRotation = PlayerRotation;

		AC_AllyWarrior* SpawnedAlly = GetWorld()->SpawnActor<AC_AllyWarrior>(AllyClass, SpawnLocation, SpawnRotation);

		if (SpawnedAlly)
		{
			SpawnParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticleSystem, SpawnLocation, SpawnRotation, true);

			if (SpawnParticleComponent)
			{
				FVector NewScale(0.7f,0.7f,0.4f); 
				SpawnParticleComponent->SetWorldScale3D(NewScale);
			}


			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticleSystem, SpawnLocation, SpawnRotation, true);
			Allies.Add(SpawnedAlly);

			auto AIController = GetWorld()->SpawnActor<AC_AllyAIController>(AC_AllyAIController::StaticClass());
			if (AIController)
			{
				AIController->Possess(SpawnedAlly);
			}
		}

		CurrentSpawnIndex++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

		for (AC_AllyWarrior* Ally : Allies)
		{
			if (Ally)
			{
				Ally->ChangeWeapon(EWeaponState::SWORD);
			}
		}
		DecalComponent->SetHiddenInGame(true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_AllySpawn::DestroySpawnActor, 1.5f, false);
	}
}

void AC_AllySpawn::DestroySpawnActor()
{
	Warrior->DeactivateSpawnCamera();
	Destroy();
}

