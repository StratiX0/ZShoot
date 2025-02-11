#include "Pawns/Zombie.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AZombie::AZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach the skeletal mesh component
	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
	if (SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
	}

	// Optionally, set the animation blueprint
	if (AnimBlueprintClass)
	{
		SkeletalMeshComponent->SetAnimInstanceClass(AnimBlueprintClass);
	}
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}