// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/PowerUpHeal.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerActor.h"
#include "Components/HealthComponent.h"


// Sets default values
APowerUpHeal::APowerUpHeal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	RingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring Mesh"));
	RingMesh->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void APowerUpHeal::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &APowerUpHeal::OnHit);

	SetStartHealAmount();
}

void APowerUpHeal::SetStartHealAmount()
{
	switch (HealType)
	{
	case EPUHealType::Option1:
		HealAmount = 25.f;
		Mesh->SetMaterial(0, MaterialsArray[0]);
		RingMesh->SetMaterial(0, MaterialsArray[0]);
		break;
	case EPUHealType::Option2:
		HealAmount = 50.f;
		Mesh->SetMaterial(0, MaterialsArray[1]);
		RingMesh->SetMaterial(0, MaterialsArray[1]);
		break;
	case EPUHealType::Option3:
		HealAmount = 100.f;
		Mesh->SetMaterial(0, MaterialsArray[2]);
		RingMesh->SetMaterial(0, MaterialsArray[2]);
		break;
	default:
		HealType = EPUHealType::Option1;
		HealAmount = 25.f;
		Mesh->SetMaterial(0, MaterialsArray[0]);
		RingMesh->SetMaterial(0, MaterialsArray[0]);
		break;
	}
}

void APowerUpHeal::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Hit"));
	if (APlayerActor* Player = Cast<APlayerActor>(OtherActor))
	{
		Player->HealthComponent->Heal(HealAmount);
		Destroy();
	}
}

void APowerUpHeal::Tick(float DeltaTime)
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

