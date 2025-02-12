#include "Components/Ammo.h"
#include "Pawns/PlayerActor.h"
#include "Widgets/PlayerHUDWidget.h"

UAmmo::UAmmo()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAmmo::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	CurrentAmmoInMag = MagCapacity;
	DisplayAmmo();
}

void UAmmo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Handle reload progress
	if (IsReloading)
	{
		RemainingReloadTime -= DeltaTime;
		if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
		{
			float ReloadProgress = FMath::Clamp((ReloadTime - RemainingReloadTime) / ReloadTime, 0.f, 1.f);
			Player->PlayerHUD->UpdateReloadBar(ReloadProgress);
		}
	}
}

void UAmmo::DisplayAmmo()
{
	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->SetAmmoValue(CurrentAmmoInMag, CurrentAmmo);
	}
}

bool UAmmo::EnoughAmmo(int AmmoNeeded)
{
	if (CurrentAmmoInMag >= AmmoNeeded)
	{
		return true;
	}
	MakeReload();
	return false;
}

void UAmmo::UseAmmo(int AmmoUsed)
{
	if (CurrentAmmoInMag <= 0 || IsReloading) return;

	CurrentAmmoInMag -= AmmoUsed;
	DisplayAmmo();
}

void UAmmo::Reload()
{
	// Finalize reload and update ammo values
	IsReloading = false;

	// Calculate remaining ammo and mag fill
	if (CurrentAmmo < MagCapacity)
	{
		CurrentAmmoInMag = CurrentAmmo;
		CurrentAmmo = 0;
	}
	else
	{
		int Difference = MagCapacity - CurrentAmmoInMag;
		CurrentAmmo -= Difference;
		CurrentAmmoInMag += Difference;
	}

	// Hide reload bar
	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->HideReloadBar();
	}

	DisplayAmmo();
}

void UAmmo::MakeReload()
{
	// If already reloading or mag is full, return
	if (IsReloading || CurrentAmmoInMag == MagCapacity) return;

	StartReload();
}

void UAmmo::StartReload()
{
	IsReloading = true;
	RemainingReloadTime = ReloadTime;

	// Start the reload timer
	ReloadTimerHandler.Invalidate();
	GetOwner()->GetWorldTimerManager().SetTimer(ReloadTimerHandler, this, &UAmmo::Reload, ReloadTime, false);

	// Show reload bar
	if (APlayerActor* Player = Cast<APlayerActor>(GetOwner()))
	{
		Player->PlayerHUD->ShowReloadBar();
	}
}

void UAmmo::FinishReload()
{
	// Finalize reload by setting flags and displaying ammo
	IsReloading = false;
	DisplayAmmo();
}

void UAmmo::AddAmmo(int AmmoToAdd)
{
	CurrentAmmo += AmmoToAdd;

	// Clamp ammo to max capacity
	if (CurrentAmmo > MaxAmmo)
	{
		CurrentAmmo = MaxAmmo;
	}

	DisplayAmmo();
}
