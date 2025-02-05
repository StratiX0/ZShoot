// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/AIZombie.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "Pawns/PlayerActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAIZombie::AAIZombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComp;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BodyMesh->SetupAttachment(BoxComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void AAIZombie::BeginPlay()
{
	Super::BeginPlay();

	PlayerActor = Cast<APlayerActor>(UGameplayStatics::GetPlayerPawn(this, 0));
	
}

// Called every frame
void AAIZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InChasingRange())
	{
		Chasing();
	}
}

bool AAIZombie::InChasingRange()
{
	if (PlayerActor)
	{
		float Distance = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());
		if (Distance <= ChasingRange)
		{
			return true;
		}
	}
	return false;
}

void AAIZombie::Chasing()
{
	if (PlayerActor)
	{
		FVector DeltaLocation = PlayerActor->GetActorLocation() - GetActorLocation();
		DeltaLocation = DeltaLocation.GetSafeNormal() * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
		DeltaLocation.Z = 0.f;
		AddActorLocalOffset(DeltaLocation, true);
	}
}

