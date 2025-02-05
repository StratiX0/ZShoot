#include "GameInstances/GILevel.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerActor.h"

void UGILevel::CreatePlayerHUD()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUDWidget>(PlayerController, PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerHUD is nullptr after creation."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController or PlayerHUDClass is nullptr."));
	}

	APlayerActor* PlayerActor = Cast<APlayerActor>(PlayerController->GetPawn());

	if (PlayerActor)
	{
		PlayerActor->PlayerHUD = Cast<UPlayerHUDWidget>(PlayerHUD);
	}
}
