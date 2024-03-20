// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/C_Hud.h"
#include "Unit/Warrior/C_Warrior.h"
#include "Blueprint/UserWidget.h"
#include "Widget/WC_PlayerUI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

void AC_Hud::DrawHUD()
{
    Super::DrawHUD();

    AC_Warrior* Warrior = Cast<AC_Warrior>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if (Warrior)
    {
        FVector2D ScreenPosition;
        if (Warrior->IsTargetLocked())
        {
            AC_Unit* TargetedEnemy = Warrior->GetTargetedEnemy();
            
            if (TargetedEnemy)
            {
                if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), TargetedEnemy->GetActorLocation(), ScreenPosition))
                {
                    float Scale = 0.02f;
                    float TextureWidthScaled = TargetedTexture->GetSizeX() * Scale;
                    float TextureHeightScaled = TargetedTexture->GetSizeY() * Scale;

                    float ScreenX = ScreenPosition.X - (TextureWidthScaled / 2);
                    float ScreenY = ScreenPosition.Y - (TextureHeightScaled / 2);

                    DrawTextureSimple(TargetedTexture, ScreenX, ScreenY, Scale);
                }
            }
        }
        else
        {
            ScreenPosition = FVector2D(Canvas->SizeX / 2, Canvas->SizeY / 2);
           
            if (!Warrior->GetIsBowCameraActive())
            {
                float Scale = 0.05f;
                float TextureWidthScaled = NontargetedCrosshair->GetSizeX() * Scale;
                float TextureHeightScaled = NontargetedCrosshair->GetSizeY() * Scale;

                float ScreenX = ScreenPosition.X - (TextureWidthScaled / 2);
                float ScreenY = ScreenPosition.Y - (TextureHeightScaled / 2);

                DrawTextureSimple(NontargetedCrosshair, ScreenX, ScreenY, Scale);
            }
            else
            {
                float Scale = 0.09f;
                float TextureWidthScaled = NontargetedCrosshair->GetSizeX() * Scale;
                float TextureHeightScaled = NontargetedCrosshair->GetSizeY() * Scale;

                float ScreenX = ScreenPosition.X - (TextureWidthScaled / 2);
                float ScreenY = ScreenPosition.Y - (TextureHeightScaled / 2);

                DrawTextureSimple(NontargetedCrosshair, ScreenX, ScreenY, Scale);
            }

      
        }
    }
}

void AC_Hud::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetOwningPlayerController();
    if (PlayerController && PlayerUIClass != nullptr)
    {
        PlayerUIInstance = CreateWidget<UWC_PlayerUI>(PlayerController, PlayerUIClass);
        if (PlayerUIInstance != nullptr)
        {
            PlayerUIInstance->AddToViewport();
        }
    }

}
