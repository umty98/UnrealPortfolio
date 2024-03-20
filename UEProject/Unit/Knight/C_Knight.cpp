// Fill out your copyright notice in the Description page of Project Settings.

//#include "Kismet/KismetSystemLibrary.h"

#include "Unit/Knight/C_Knight.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/C_Sword.h"
#include "Kismet/GameplayStatics.h"

//using namespace WeaponState;

// Sets default values
AC_Knight::AC_Knight()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(L"/Script/Engine.SkeletalMesh'/Game/Model/Knight/Textures/Knight_D_Pelegrini.Knight_D_Pelegrini'");

	GetMesh()->SetSkeletalMesh(mesh.Object);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -87.5f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AC_Knight::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AC_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFlip)
		bIsFlip = false;

	if (bIsLoop && WeaponType == EWeaponState::RIFLE)
		GetCurrentWeapon()->Attack();
}

// Called to bind functionality to input
void AC_Knight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AC_Knight::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight",   this, &AC_Knight::OnMoveRight);

	PlayerInputComponent->BindAxis("RotateHorizontal", this, &AC_Knight::OnRotateHorizontal);
	PlayerInputComponent->BindAxis("RotateVertical",   this, &AC_Knight::OnRotateVertical);

	PlayerInputComponent->BindAction("Jump",   EInputEvent::IE_Pressed, this, &AC_Knight::OnJump);
	PlayerInputComponent->BindAction("Sword",  EInputEvent::IE_Pressed, this, &AC_Knight::OnSwordKey);
	PlayerInputComponent->BindAction("Rifle",  EInputEvent::IE_Pressed, this, &AC_Knight::OnRifleKey);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AC_Knight::OnAttack);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Released, this, &AC_Knight::OffAttack);
}

void AC_Knight::OnMoveForward(float Axis)
{
	if (!bCanMove)
		return;

	AddMovementInput(Camera->GetForwardVector().GetSafeNormal2D() * MoveSpeed * GetWorld()->DeltaTimeSeconds, Axis);
}

void AC_Knight::OnMoveRight(float Axis)
{
	if (!bCanMove)
		return;

	AddMovementInput(Camera->GetRightVector().GetSafeNormal2D() * MoveSpeed * GetWorld()->DeltaTimeSeconds, Axis);
}

void AC_Knight::OnRotateHorizontal(float Axis)
{
	AddControllerYawInput(Axis * RotSpeed * GetWorld()->DeltaTimeSeconds);
}

void AC_Knight::OnRotateVertical(float Axis)
{
	AddControllerPitchInput(Axis * RotSpeed * GetWorld()->DeltaTimeSeconds);
}

void AC_Knight::OnJump()
{
	Jump();

	if (JumpMaxCount > JumpCurrentCount)
		bIsFlip = true;
}

void AC_Knight::OnSwordKey()
{
	ChangeWeapon(EWeaponState::SWORD);
}

void AC_Knight::OnRifleKey()
{
	ChangeWeapon(EWeaponState::RIFLE);
}
