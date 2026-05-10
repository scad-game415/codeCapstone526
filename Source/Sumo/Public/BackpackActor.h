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
    TSubclassOf<AActor> ItemClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Quantity = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS()
class SUMO_API ABackpackActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABackpackActor();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryUpdated OnInventoryUpdated;

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

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItemToSlot(int SlotIndex, FBackpackItem Item);

	UFUNCTION(BlueprintPure, Category = "Inventory")
    FBackpackItem GetItemInSlot(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
    void SetItemInSlot(int SlotIndex, FBackpackItem NewItem);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
    void ClearSlot(int SlotIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BackpackMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool HintVisible = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanPressE = false;

	UPROPERTY(ReplicatedUsing = OnRep_Inventory, EditAnywhere, Category = "Inventory")
	TArray<FBackpackItem>Inventory; //this should be the struct for items we end up using
	//later n note - i made my own, should be able to transfer between easier, self reminder to optimize later
	
	UPROPERTY(EditAnywhere, Category="Inventory")
	int InventorySize = 9;

	UFUNCTION()
	void OnRep_Inventory() { OnInventoryUpdated.Broadcast(); };
	
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

	void CheckIfSettled();

	FTimerHandle SettledTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
