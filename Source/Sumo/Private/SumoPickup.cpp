// Fill out your copyright notice in the Description page of Project Settings.

//////////////////////////////////////////////////////////////////////////////////
/////// Prof Lindsey SumoPickup Example class. Don't confuse with your own.///////
//////////////////////////////////////////////////////////////////////////////////

#include "SumoPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ASumoPickup::ASumoPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the collision sphere as root so the whole actor moves together
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(100.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionSphere);

	// Create the visible mesh and attach it to the collision sphere
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(CollisionSphere);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASumoPickup::BeginPlay()
{
	Super::BeginPlay();

	// Save starting height for the bob offset
	StartZ = GetActorLocation().Z;

	// Bind the overlap event
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASumoPickup::OnOverlapBegin);
}

void ASumoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Spin the mesh
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += RotationSpeed * DeltaTime;
	SetActorRotation(Rotation);

	// Bob up and down with a sine wave
	RunningTime += DeltaTime;
	FVector Location = GetActorLocation();
	Location.Z = StartZ + BobAmplitude * FMath::Sin(RunningTime * BobSpeed * 2.0f * PI);
	SetActorLocation(Location);
}

void ASumoPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Only react to pawns (characters)
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		// Play the pickup sound
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}

		// Call the Blueprint event so subclasses can add effects/scoring
		OnPickedUp(OtherActor);

		// Remove the pickup from the world
		SetLifeSpan(.1);
	}
}
