// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/PowerUpAmmo.h"

#include "Components/Ammo.h"
#include "Pawns/PlayerActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APowerUpAmmo::APowerUpAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	RingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring Mesh"));
	RingMesh->SetupAttachment(Mesh);

	// Configure collision settings
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	RingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RingMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void APowerUpAmmo::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &APowerUpAmmo::OnHit);

	AmmoAmount = FMath::RandRange(10, 50);
}

// Called every frame
void APowerUpAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the actor
	FRotator Rotation = FRotator(0.f, RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this), 0.f);
	Mesh->AddLocalRotation(Rotation);

	FVector NewLocation = FVector::ZeroVector;
	float DeltaHeight = FMath::Sin(UGameplayStatics::GetTimeSeconds(this) * AnimationSpeed) * AnimationHeight;
	NewLocation.Z += DeltaHeight;
	Mesh->AddRelativeLocation(NewLocation);
}

void APowerUpAmmo::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
 FVector NormalImpulse, const FHitResult& Hit)
{
	if (APlayerActor* Player = Cast<APlayerActor>(OtherActor))
	{
		if (!Player->AmmoComponent->IsFull())
		{
			Player->AmmoComponent->AddAmmo(AmmoAmount);
			Destroy();
		}
	}
}

