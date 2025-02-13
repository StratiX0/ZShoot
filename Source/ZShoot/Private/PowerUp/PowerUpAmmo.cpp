#include "PowerUp/PowerUpAmmo.h"
#include "Components/Ammo.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerClass.h"

// Constructor to set default values
APowerUpAmmo::APowerUpAmmo()
{
	// Enable ticking every frame
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	RingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring Mesh"));
	RingMesh->SetupAttachment(Mesh);

	// Initialize collision settings
	InitializeCollision();
}

// Called when the game starts or when spawned
void APowerUpAmmo::BeginPlay()
{
	Super::BeginPlay();

	// Bind the hit event to the OnHit function
	Mesh->OnComponentHit.AddDynamic(this, &APowerUpAmmo::OnHit);

	// Randomize the ammo amount within a specified range
	AmmoAmount = FMath::RandRange(10, 50);
}

// Called every frame
void APowerUpAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the mesh with constant speed
	FRotator Rotation = FRotator(0.f, RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this), 0.f);
	Mesh->AddLocalRotation(Rotation);

	// Create a sine wave-based bounce effect
	float DeltaHeight = FMath::Sin(UGameplayStatics::GetTimeSeconds(this) * AnimationSpeed) * AnimationHeight;
	Mesh->AddRelativeLocation(FVector(0.f, 0.f, DeltaHeight));
}

// Collision handler when the power-up hits something
void APowerUpAmmo::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (APlayerClass* Player = Cast<APlayerClass>(OtherActor))
	{
		// If the player's ammo isn't full, add the power-up's ammo
		if (!Player->AmmoComponent->IsFull())
		{
			Player->AmmoComponent->AddAmmo(AmmoAmount);
			Destroy();  // Destroy the power-up after it's picked up
		}
	}
}

// Helper function to initialize the collision settings for meshes
void APowerUpAmmo::InitializeCollision()
{
	// Configure the mesh's collision
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// Configure the ring mesh's collision similarly
	RingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RingMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}
