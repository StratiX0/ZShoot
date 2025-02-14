// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUp/PowerUpHeal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "Pawns/PlayerClass.h"

// Sets default values
APowerUpHeal::APowerUpHeal()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialize Mesh components
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

	// Set the initial heal amount and materials
	SetStartHealAmount();
	
	// Store the initial height
	InitialHeight = Mesh->GetRelativeLocation().Z;
}

void APowerUpHeal::SetStartHealAmount()
{
	// Create a mapping of EPUHealType to HealAmount and materials
	TMap<EPUHealType, TPair<float, UMaterial*>> HealMapping = {
		{EPUHealType::Option1, TPair<float, UMaterial*>(25.f, MaterialsArray[0])},
		{EPUHealType::Option2, TPair<float, UMaterial*>(50.f, MaterialsArray[1])},
		{EPUHealType::Option3, TPair<float, UMaterial*>(100.f, MaterialsArray[2])}
	};

	// Set the heal amount and materials based on HealType
	if (HealMapping.Contains(HealType))
	{
		HealAmount = HealMapping[HealType].Key;
		Mesh->SetMaterial(0, HealMapping[HealType].Value);
		RingMesh->SetMaterial(0, HealMapping[HealType].Value);
	}
	else
	{
		// Default to Option1
		HealType = EPUHealType::Option1;
		HealAmount = 25.f;
		Mesh->SetMaterial(0, MaterialsArray[0]);
		RingMesh->SetMaterial(0, MaterialsArray[0]);
	}
}

void APowerUpHeal::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         FVector NormalImpulse, const FHitResult& Hit)
{
	if (APlayerClass* Player = Cast<APlayerClass>(OtherActor))
	{
		// Heal the player if not at full health
		if (!Player->HealthComponent->IsFullHealth())
		{
			Player->HealthComponent->Heal(HealAmount);
			Destroy();
		}
	}
}

void APowerUpHeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the mesh component
	FRotator Rotation = FRotator(0.f, RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this), 0.f);
	Mesh->AddLocalRotation(Rotation);

	// Animate the mesh's height around its initial height
	static bool bGoingUp = true;
	static float CurrentHeightOffset = 0.0f;
	float DeltaMove = DeltaTime * AnimationSpeed;

	if (bGoingUp)
	{
		CurrentHeightOffset += DeltaMove;
		if (CurrentHeightOffset >= AnimationHeight)
		{
			CurrentHeightOffset = AnimationHeight;
			bGoingUp = false;
		}
	}
	else
	{
		CurrentHeightOffset -= DeltaMove;
		if (CurrentHeightOffset <= 0.0f)
		{
			CurrentHeightOffset = 0.0f;
			bGoingUp = true;
		}
	}

	FVector NewLocation = Mesh->GetRelativeLocation();
	NewLocation.Z = InitialHeight + CurrentHeightOffset;
	Mesh->SetRelativeLocation(NewLocation);
}

void APowerUpHeal::SetRandomHealType()
{
	TArray<EPUHealType> HealTypes = { EPUHealType::Option1, EPUHealType::Option2, EPUHealType::Option3 };
	int32 RandomIndex = FMath::RandRange(0, HealTypes.Num() - 1);
	HealType = HealTypes[RandomIndex];

	SetStartHealAmount();
}


