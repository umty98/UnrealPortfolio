// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Boss/C_Boss.h"
#include "Object/C_Weapon.h"
#include "Object/C_Orb.h"
#include "Kismet/GameplayStatics.h"
#include "Unit/Boss/C_BossNearShield.h"
#include "Unit/Boss/C_BossMiddleAttack.h"
#include "Unit/Boss/C_BossFallingSpear.h"
#include "Unit/Boss/C_BossFollowingSpear.h"
#include "NiagaraComponent.h"
#include "Widget/C_Hud.h"
#include "Widget/WC_PlayerUI.h"

AC_Boss::AC_Boss()
{
	TeamID = ETeamAttitude::Hostile;
}

void AC_Boss::BeginPlay()
{
	Super::BeginPlay();

	ChangeWeapon(EWeaponState::ORB);

	CachedHud = GetHud();

	/*
	if (CachedHud)
	{
		FString HudName = CachedHud->GetName();
		UKismetSystemLibrary::PrintString(this, HudName);
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "null");
	}
	*/

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		//CachedHud->PlayerUIInstance->MiniToWorld(false);
		CachedHud->PlayerUIInstance->UpdateBossHpBox(true);
	}

}

void AC_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CachedHud && CachedHud->PlayerUIInstance)
	{
		CachedHud->PlayerUIInstance->UpdateBossHp(GetHp(), MaxHp);
	}

	

}

void AC_Boss::ShieldActive()
{
	shieldSkill.bCanUseSkill = false;
	//shieldSkill.bSKillUsed = true;

	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Orb* orb = Cast<AC_Orb>(CurrentWeapon);
		if (orb)
		{
			orb->ActiveShield();
		}
	}

	shield = GetWorld()->SpawnActor<AC_BossNearShield>(ShieldClass, this->GetActorLocation(),this->GetActorRotation());
	shield->SetOwnerController(GetController());
	shield->SetOwner(this);
	shield->GetShieldActiveComponenent()->Activate(true);

}

void AC_Boss::DestroyShield()
{
	if (shield && !shield->IsPendingKill())
	{
		shield->Destroy();
		shield = nullptr;
	}
	shieldSkill.bSKillUsed = true;
	GetWorld()->GetTimerManager().SetTimer(shieldSkill.BossSkillTimerHandle, this, &AC_Boss::SetUseShieldSkill, 8.0f, false);
}

void AC_Boss::ShieldExplosive()
{
	shield->GetShieldExplosiveComponenent()->Activate(true);
}

void AC_Boss::ActivateShieldCollision()
{
	shield->ActivateCollision();
}

void AC_Boss::DeactivateShieldCollision()
{
	shield->DeactivateCollision();
}

void AC_Boss::SetUseShieldSkill()
{
	if (shieldSkill.bCanUseSkill == false)
	{
		shieldSkill.bCanUseSkill = true;
		shieldSkill.bSKillUsed = false;
	}
}

void AC_Boss::MiddleAttackActive()
{
	middleSkill.bCanUseSkill = false;
	//middleSkill.bSKillUsed = true;

	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Orb* orb = Cast<AC_Orb>(CurrentWeapon);
		if (orb)
		{
			orb->ActiveMiddleAttack();
		}
	}

	middle = GetWorld()->SpawnActor<AC_BossMiddleAttack>(MiddleClass, this->GetActorLocation(), this->GetActorRotation());
	middle->SetOwnerController(GetController());
	middle->SetOwner(this);

	//->GetShieldActiveComponenent()->Activate(true);

}

void AC_Boss::DestroyMiddleAttack()
{
	if (middle && !middle->IsPendingKill())
	{
		middle->Destroy();
		middle = nullptr;
	}
	middleSkill.bSKillUsed = true;
	GetWorld()->GetTimerManager().SetTimer(middleSkill.BossSkillTimerHandle, this, &AC_Boss::SetUseMiddleSkill, 5.5f, false);
}

void AC_Boss::ActivateMiddleCollision()
{
	middle->ActivateCollision();
}

void AC_Boss::DeactivateMiddleCollision()
{
	middle->DeactivateCollision();
}

void AC_Boss::SetUseMiddleSkill()
{
	if (middleSkill.bCanUseSkill == false)
	{
		middleSkill.bCanUseSkill = true;
		middleSkill.bSKillUsed = false;
	}
}

void AC_Boss::FallingSpearActive()
{
	//bCanUseSkill = false;
	fallingSpearSkill.bCanUseSkill = false;
	bCanUseSkill = false;
	//fallingSpearSkill.bSKillUsed = true;

	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Orb* orb = Cast<AC_Orb>(CurrentWeapon);
		if (orb)
		{
			orb->ActiveFallingSpear();
		}
	}

	FVector SpawnLocation = WarriorLocation;
	SpawnLocation.Z -= 80.0f;

	fallingSpear = GetWorld()->SpawnActor<AC_BossFallingSpear>(FallingSpearClass, SpawnLocation, this->GetActorRotation());
	fallingSpear->SetOwnerController(GetController());
	fallingSpear->SetOwner(this);


}

void AC_Boss::FallingSpearSpawn()
{
	fallingSpear->ActivateNiagara();
}

void AC_Boss::ActivateSpearCollision()
{
	fallingSpear->ActivateCollision();
}

void AC_Boss::DeactivateSpearCollision()
{
	fallingSpear->DeactivateCollision();
	DestroyFallingSpear();
}

void AC_Boss::DestroyFallingSpear()
{
	if (fallingSpear && !fallingSpear->IsPendingKill())
	{
		fallingSpear->Destroy();
		fallingSpear = nullptr;
	}
	fallingSpearSkill.bSKillUsed = true;
	GetWorld()->GetTimerManager().SetTimer(fallingSpearSkill.BossSkillTimerHandle, this, &AC_Boss::SetUseSpearSkillTrue, 1.5f, false);

}

void AC_Boss::SetUseSpearSkillTrue()
{
	if (fallingSpearSkill.bCanUseSkill == false)
	{
		fallingSpearSkill.bCanUseSkill = true;
		fallingSpearSkill.bSKillUsed = false;
	}
}

void AC_Boss::FollowingSpearActive()
{
	followingSpearSkill.bCanUseSkill = false;
	//followingSpearSkill.bSKillUsed = true;

	AC_Weapon* CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon)
	{
		AC_Orb* orb = Cast<AC_Orb>(CurrentWeapon);
		if (orb)
		{
			orb->ActiveFollowingSpear();
		}
	}

	FVector SpawnLocation = this->GetActorLocation();
	SpawnLocation.Z += 150.0f;

	followingSpear = GetWorld()->SpawnActor<AC_BossFollowingSpear>(FollowingSpearClass, SpawnLocation, this->GetActorRotation());
	followingSpear->SetOwnerController(GetController());
	followingSpear->SetOwner(this);

	GetWorld()->GetTimerManager().SetTimer(followingSpearSkill.BossSkillTimerHandle, this, &AC_Boss::SetIsFollowingSkillUsedTrue, 2.5f, false);
	GetWorld()->GetTimerManager().SetTimer(followingSpearSkill.BossSkillTimerHandle, this, &AC_Boss::SetUseFollowingSpearTrue, 3.5f, false);
}

void AC_Boss::SetUseFollowingSpearTrue()
{
	if (followingSpearSkill.bCanUseSkill == false)
	{
		followingSpearSkill.bCanUseSkill = true;
		followingSpearSkill.bSKillUsed = false;
	}
}



void AC_Boss::SetIsFollowingSkillUsedTrue()
{
	followingSpearSkill.bSKillUsed = true;
}

bool AC_Boss::GetCanUseFarSkill()
{
	if (this->CurHp > this->MaxHp / 2)
		return fallingSpearSkill.bCanUseSkill;
	else
		return followingSpearSkill.bCanUseSkill;


	return false;
}

void AC_Boss::CanUseSkillTrue()
{
	//UKismetSystemLibrary::PrintString(GetWorld(), "delayfalse");
	if (bCanUseSkill == false)
	{
		bCanUseSkill = true;
	}
}


AC_Hud* AC_Boss::GetHud() const
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		return Cast<AC_Hud>(PC->GetHUD());
	}
	return nullptr;
}

