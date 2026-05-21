// Fill out your copyright notice in the Description page of Project Settings.


#include "BackpackActor.h"
#include "SumoPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABackpackActor::ABackpackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BackpackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackpackMesh"));
	RootComponent = BackpackMesh;

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);

	BackpackMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABackpackActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		Inventory.SetNum(InventorySize);
	}
}


// Called every frame
void ABackpackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABackpackActor::EquipBackpack(ACharacter* NewOwner)
{
	if (!HasAuthority() || !NewOwner) return;

	BackpackMesh->SetSimulatePhysics(false);
	SetReplicateMovement(false);

	OriginalScale = NewOwner->GetActorScale3D();
	NewOwner->SetActorScale3D(EncumberedScale);

	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	AttachToComponent(NewOwner->GetMesh(), AttachmentRules, FName("SpineSocket"));
	//if our character doesn't have a spine socket this might break, remember to request for that
	SetOwner(NewOwner);
	Multicast_OnEquipped(NewOwner);

	SetActorRelativeLocation(socketOffset);
	SetActorRelativeRotation(socketRotation);

	if (UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(NewOwner->GetCharacterMovement()))
	{
		OriginalWalkSpeed = MovementComp->MaxWalkSpeed;
		MovementComp->MaxWalkSpeed = EncumberedSpeed;
	}

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABackpackActor::Multicast_OnEquipped_Implementation(ACharacter* NewOwner)
{
	if (!NewOwner) return;

	NewOwner->SetActorScale3D(EncumberedScale);

	if (UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(NewOwner->GetCharacterMovement()))
	{
		OriginalWalkSpeed = MovementComp->MaxWalkSpeed;
		MovementComp->MaxWalkSpeed = EncumberedSpeed;
	}

	BackpackMesh->SetSimulatePhysics(false);
	SetReplicateMovement(false);
	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	AttachToComponent(NewOwner->GetMesh(), AttachRules, FName("SpineSocket"));
}

void ABackpackActor::DropBackpack() 
{
	if (!HasAuthority()) return;

	ACharacter* CurrentOwnerChar = Cast<ACharacter>(GetOwner());

	if (UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(CurrentOwnerChar->GetCharacterMovement())) 
	{
		MovementComp->MaxWalkSpeed = OriginalWalkSpeed;
	}
	SetReplicateMovement(true);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetOwner(nullptr);


	CurrentOwnerChar->SetActorScale3D(OriginalScale);

	Multicast_OnDropped(CurrentOwnerChar);

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BackpackMesh->SetSimulatePhysics(true);

	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetWorldTimerManager().SetTimer(SettledTimerHandle, this, &ABackpackActor::CheckIfSettled, 0.5f, true, 2.0f);
}

void ABackpackActor::Multicast_OnDropped_Implementation(ACharacter* Character)
{
	if (!Character) return;

	Character->SetActorScale3D(OriginalScale);

	if (UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement()))
	{
		MovementComp->MaxWalkSpeed = 600; //trying to find a way to make this work a little better but for now manually setting
	}
	SetReplicateMovement(true);

	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	BackpackMesh->SetSimulatePhysics(true);
	BackpackMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABackpackActor::CheckIfSettled()
{
	if (!HasAuthority() || !BackpackMesh) return;

	if (BackpackMesh->GetPhysicsLinearVelocity().Size() < 10.0f)
	{
		FRotator CurrentRot = GetActorRotation();
		SetActorRotation(FRotator(0.0f, CurrentRot.Yaw, 0.0f));

		GetWorldTimerManager().ClearTimer(SettledTimerHandle);
		BackpackMesh->SetSimulatePhysics(false);
	}
}

void ABackpackActor::BackpackPlayerInteraction(ACharacter* InteractingPlayer) 
{
	if (!HasAuthority()) return;

	//inventory logic
	ASumoPlayerController* PC = Cast<ASumoPlayerController>(InteractingPlayer->GetController());
	if (PC)
	{
		PC->Client_OpenBackpack(this);
	}
}

void ABackpackActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABackpackActor, Inventory);
}

bool ABackpackActor::AddItemToSlot(int SlotIndex, FBackpackItem Item)
{
	if (!HasAuthority() || !Inventory.IsValidIndex(SlotIndex)) return false;

	if (Inventory[SlotIndex].ItemClass == nullptr)
	{
		Inventory[SlotIndex] = Item;
		return true;
	}
	return false;
}

FBackpackItem ABackpackActor::GetItemInSlot(int SlotIndex)
{
	if (Inventory.IsValidIndex(SlotIndex))
	{
		return Inventory[SlotIndex];
	}
	return FBackpackItem();
}

void ABackpackActor::SetItemInSlot(int SlotIndex, FBackpackItem NewItem)
{
	if (HasAuthority() && Inventory.IsValidIndex(SlotIndex))
	{
		Inventory[SlotIndex] = NewItem;

		if (GetNetMode() != NM_DedicatedServer) //this is only needed for listen server so i have no idea if its necessary
		{
			OnRep_Inventory();
		}
	}
}

void ABackpackActor::ClearSlot(int SlotIndex)
{
	if (HasAuthority() && Inventory.IsValidIndex(SlotIndex))
	{
		Inventory[SlotIndex] = FBackpackItem();

		if (GetNetMode() != NM_DedicatedServer) //but better safe than sorry i guess
		{
			OnRep_Inventory();
		}
	}
}

