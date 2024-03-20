// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Enemy/C_Enemy.h"
#include "Particles/ParticleSystemComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/EnemyAI/C_PatrolComponent.h"

AC_Enemy::AC_Enemy()
{
    TeamID = ETeamAttitude::Hostile;

    PatrolComponent = CreateDefaultSubobject<UC_PatrolComponent>("Patrol");

}

void AC_Enemy::BeginPlay()
{
    Super::BeginPlay();

    ChangeWeapon(EWeaponState::SWORD);

    if (bIsLeader)
        LeaderStartLocation = GetActorLocation();
    
    /*
    AActor* MyOwner = GetOwner();
    if (MyOwner != nullptr)
    {
        FString OwnerName = MyOwner->GetName();
        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Current Owner Name: ") + OwnerName);
    }
    else
    {
        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Current Owner is nullptr"));
    }

    AController* CurrentController = GetController();
    if (CurrentController != nullptr)
    {
        FString ControllerName = CurrentController->GetName();
        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Controlled by: ") + ControllerName);
    }
    else
    {
        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("No controller is assigned yet."));
    }
    */

}

void AC_Enemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bCanMove)
    {
        GetCharacterMovement()->MaxWalkSpeed = 0;
    }
    else
        GetCharacterMovement()->MaxWalkSpeed = 300;
        
}

void AC_Enemy::LaunchIntoAirQSKill()
{
	FVector LaunchVelocity(0, 0, 900); 
	LaunchCharacter(LaunchVelocity, true, true);
    bIsFloating = true;
    bIsHitByQSkill = true;
    GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, -30.0f));
   
}

void AC_Enemy::KnockBack(FVector Direction)
{
    FVector KnockbackDirection = Direction;
    KnockbackDirection.Normalize(); 

    float KnockbackStrength = 500.0f; 
    FVector LaunchVelocity = KnockbackDirection * KnockbackStrength;
    LaunchVelocity.Z = 200.0f; 

    LaunchCharacter(LaunchVelocity, true, true);
}

void AC_Enemy::EnemyBounce()
{
    FVector LaunchVelocity(0, 0, 400);
    LaunchCharacter(LaunchVelocity, true, true);
}

void AC_Enemy::ApplyForce(FVector Force)
{
    if (bIsFloating)
    {
        GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, -90.0f));
        LaunchCharacter(Force, true, true);
    }
}

void AC_Enemy::LaunchIntoAirESkill()
{
    FVector LaunchVelocity(0, 0, 600);
    LaunchCharacter(LaunchVelocity, true, true);
    bIsFloating = true;
    bIsHitByESkill = true;
    GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, -60.0f));
}

void AC_Enemy::LookAround()
{
    if(LookAroundMontage)
        PlayAnimMontage(LookAroundMontage);
}


void AC_Enemy::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    bIsFloating = false;

    if (bIsHitByQSkill)
    {
        GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

        if (QSKillLand)
        {
            FTransform SpawnTransform = this->GetTransform();
            FVector SpawnLocation = SpawnTransform.GetLocation();
   
            SpawnLocation.Z -= 80.0f;
            SpawnTransform.SetLocation(SpawnLocation);

            UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                QSKillLand,
                SpawnTransform
            );

            if (ParticleSystemComponent)
            {
                ParticleSystemComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
            }
        }


        bIsHitByQSkill = false;
    }

    if (bIsHitByESkill)
    {
        GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

        if (QSKillLand)
        {
            FTransform SpawnTransform = this->GetTransform();
            FVector SpawnLocation = SpawnTransform.GetLocation();

            SpawnLocation.Z -= 80.0f;
            SpawnTransform.SetLocation(SpawnLocation);

            UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                QSKillLand,
                SpawnTransform
            );

            if (ParticleSystemComponent)
            {
                ParticleSystemComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
            }
        }

        bIsHitByESkill = false;
    }

}

void AC_Enemy::SetLeader(AC_Enemy* NewLeader)
{
    LeaderEnemy = NewLeader;
}

void AC_Enemy::PlayReactMontage()
{
    if (ReactMontage)
        PlayAnimMontage(ReactMontage);
}

