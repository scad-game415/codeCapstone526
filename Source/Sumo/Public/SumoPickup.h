// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SumoPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USoundBase;

/**
 *  Demo Pickup Class — single player only, does NOT replicate.
 *  Covers the basics: components, UPROPERTY, overlap events, and
 *  BlueprintImplementableEvent for subclass extension.
 */
UCLASS()
class SUMO_API ASumoPickup : public AActor
{
	GENERATED_BODY()

	/** Visible mesh for the pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* PickupMesh;

	/** Sphere collision used as the overlap trigger */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USphereComponent* CollisionSphere;

public:

	ASumoPickup();

protected:

	/** How fast the pickup spins (degrees per second) */
	UPROPERTY(EditAnywhere, Category="Pickup")
	float RotationSpeed = 90.0f;

	/** How far the pickup bobs up and down (cm) */
	UPROPERTY(EditAnywhere, Category="Pickup")
	float BobAmplitude = 15.0f;

	/** How fast the pickup bobs (cycles per second) */
	UPROPERTY(EditAnywhere, Category="Pickup")
	float BobSpeed = 2.0f;

	/** Sound to play when collected */
	UPROPERTY(EditAnywhere, Category="Pickup")
	USoundBase* PickupSound;

	/** Starting Z location, saved at BeginPlay for bobbing offset */
	float StartZ = 0.0f;

	/** Running time accumulator for the bob sine wave */
	float RunningTime = 0.0f;

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

protected:

	/** Called when something overlaps the collision sphere */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	/** Blueprint hook for subclasses to add effects or scoring */
	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnPickedUp(AActor* Collector);
};
