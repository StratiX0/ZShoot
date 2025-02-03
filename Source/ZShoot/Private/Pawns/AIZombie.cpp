// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/AIZombie.h"
#include "Components/BoxComponent.h"

// Sets default values
AAIZombie::AAIZombie()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComp;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BodyMesh->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void AAIZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

