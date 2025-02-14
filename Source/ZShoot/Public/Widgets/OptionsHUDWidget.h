// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "OptionsHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZSHOOT_API UOptionsHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<UButton> ReturnButton;
	
	UFUNCTION()
	void OnReturnClicked();

	virtual void NativeConstruct() override;

};
