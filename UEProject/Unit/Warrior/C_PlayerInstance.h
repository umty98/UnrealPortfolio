// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C_PlayerInstance.generated.h"

/**
 * 
 */
UCLASS()
class UEPROJECT_API UC_PlayerInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    class Builder {
    private:
        UC_PlayerInstance* Instance;

    public:
        Builder(UC_PlayerInstance* ExistingInstance) : Instance(ExistingInstance) {}

        Builder& SetPlayerLevel(int32 Level) {
            Instance->PlayerLevel = Level;
            return *this;
        }

        Builder& SetPlayerCurExp(float CurExp) {
            Instance->PlayerCurExp = CurExp;
            return *this;
        }

        Builder& SetPlayerMaxExp(float MaxExp) {
            Instance->PlayerMaxExp = MaxExp;
            return *this;
        }

        Builder& SetPlayerCurHP(float CurHP) {
            Instance->PlayerCurHP = CurHP;
            return *this;
        }

        Builder& SetPlayerMaxHP(float MaxHP) {
            Instance->PlayerMaxHP = MaxHP;
            return *this;
        }

        Builder& SetPlayerCurMP(float CurMP) {
            Instance->PlayerCurMP = CurMP;
            return *this;
        }

        Builder& SetPlayerMaxMP(float MaxMP) {
            Instance->PlayerMaxMP = MaxMP;
            return *this;
        }

        UC_PlayerInstance* Build() {
            return Instance;
        }
    };

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player State")
    int32 PlayerLevel;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player State")
    float PlayerCurExp;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player State")
    float PlayerMaxExp;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player State")
    float PlayerCurHP;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player State")
    float PlayerMaxHP;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player State")
    float PlayerCurMP;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player State")
    float PlayerMaxMP;

protected:
    UC_PlayerInstance() {}
};