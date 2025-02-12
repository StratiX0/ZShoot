// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GMLevel.generated.h"

/**
 * 
 */
UCLASS()
class ZSHOOT_API AGMLevel : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	void InitializeGame();
};
