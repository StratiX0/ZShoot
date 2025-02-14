#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpAmmo.generated.h"

UCLASS()
class ZSHOOT_API APowerUpAmmo : public AActor
{
	GENERATED_BODY()
	
public:
	// Constructor to set default values
	APowerUpAmmo();

	// Override Tick function for custom behavior
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Static Mesh for the main object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	// Static Mesh for the ring around the object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RingMesh;

	// Ammo amount to be provided to the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo Properties", meta = (AllowPrivateAccess = "true"))
	int AmmoAmount = 25;

	// Rotation speed for the object
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 45.f;

	// Height for animation bounce
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float AnimationHeight = 1.f;

	// Speed of the bouncing animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float AnimationSpeed = 5.f;

	// Function to handle collision events
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Helper function to initialize collision
	void InitializeCollision();

	float InitialHeight;

};
