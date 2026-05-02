// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "burgerConsumable.generated.h"

UCLASS()
class SUMO_API AburgerConsumable : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category="Burger", meta=(ALlowPrivateAccess="true"))
	UStaticMeshComponent* BurgerMesh;
	
public:	
	// Sets default values for this actor's properties
	AburgerConsumable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Burger|Burger|Burger")
	bool isEdible = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
