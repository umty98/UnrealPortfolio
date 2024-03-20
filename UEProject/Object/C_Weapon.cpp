// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/C_Weapon.h"

#include "Unit/Warrior/C_Warrior.h"
#include "Unit/C_Unit.h"
#include "Animation/AnimMontage.h"
#include "Unit/MousePet/C_MousePet.h"

// Sets default values
AC_Weapon::AC_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	//mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AC_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<FName> rowNames = SwordComboDataTable->GetRowNames();

	for (FName name : rowNames)
	{
		FComboData* data = SwordComboDataTable->FindRow<FComboData>(name, name.ToString());

		SwordComboData.Add(data);
	}

	Owner = Cast<AC_Unit>(GetOwner());
	warrior = Cast<AC_Warrior>(GetOwner());
}

// Called every frame
void AC_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_Weapon::Attack()
{
	if (Owner->GetIsAttacking())
		return;

	if (Owner->GetMesh()->GetAnimInstance()->Montage_IsPlaying(SwordComboData[0]->Animation))
		return;

	Owner->PlayAnimMontage(SwordComboData[0]->Animation);
}

void AC_Weapon::EndAttack()
{
}

void AC_Weapon::BeginCombo()
{
}

void AC_Weapon::AttachWeapon()
{
	AttachToComponent
	(
		Owner->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		EquipSocketName
	);
}

void AC_Weapon::DetachWeapon()
{
	AttachToComponent
	(
		Owner->GetMesh(), 
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), 
		UnEquipSocketName
	);
}

void AC_Weapon::AttachWeapon(USceneComponent* Parent)
{
	AttachToComponent
	(
		Parent, 
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		UnEquipSocketName
	);
}

