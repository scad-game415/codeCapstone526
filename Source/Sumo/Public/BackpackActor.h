// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h" 
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"        
#include "GameFramework/Character.h"
#include "Templates/SubclassOf.h"

#include "BackpackActor.generated.h"

USTRUCT(BlueprintType)
struct FBackpackItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
};

UCLASS()
class SUMO_API ABackpackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackpackActor();

	UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnEquipped(ACharacter* NewOwner);

	UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnDropped(ACharacter* Character);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
	const override;

	UFUNCTION(BlueprintCallable, Category="Backpack")
	void EquipBackpack(ACharacter* NewOwner);

	UFUNCTION(BlueprintCallable, Category="Backpack")
	void DropBackpack();

	UFUNCTION(BlueprintCallable, Category="Backpack")
	void BackpackPlayerInteraction(ACharacter* InteractingPlayer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BackpackMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* InteractionVolume;

	UPROPERTY(ReplicatedUsing = OnRep_Inventory)
	TArray<FBackpackItem>Inventory; //this should be the struct for items we end up using

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool HintVisible = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanPressE = false;

	UFUNCTION()
	void OnRep_Inventory();
	
	UPROPERTY(EditDefaultsOnly, Category="Backpack|Defaults")
	float OriginalWalkSpeed = 600;
	UPROPERTY(EditDefaultsOnly, Category="Backpack|Defaults")
	FVector OriginalScale = FVector(1 ,1 ,1);

	UPROPERTY(EditDefaultsOnly, Category="Backpack|Effects")
	float EncumberedSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category="Backpack|Effects")
	FVector EncumberedScale = FVector(1.2f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category="Backpack|Positioning")
	FVector socketOffset;

	UPROPERTY(EditDefaultsOnly, Category="Backpack|Positioning")
	FRotator socketRotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
