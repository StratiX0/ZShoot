// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GILevel.generated.h"

class UPlayerHUDWidget;
/**
 * 
 */
UCLASS()
class ZSHOOT_API UGILevel : public UGameInstance
{
	GENERATED_BODY()

public:
	void CreatePlayerHUD();

protected:

private:
	UPROPERTY(EditAnywhere, Category = "HUD Properties")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;
	UPROPERTY(EditAnywhere, Category = "HUD Properties")
	UPlayerHUDWidget* PlayerHUD;
	
};
