// Fill out your copyright notice in the Description page of Project Settings.


#include "BackpackActor.h"
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
}

// Called when the game starts or when spawned
void ABackpackActor::BeginPlay()
{
	Super::BeginPlay();
	
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

	OriginalScale = NewOwner->GetActorScale3D();
	NewOwner->SetActorScale3D(EncumberedScale);

	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

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
	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

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

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetOwner(nullptr);

	CurrentOwnerChar->SetActorScale3D(OriginalScale);

	Multicast_OnDropped(CurrentOwnerChar);

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BackpackMesh->SetSimulatePhysics(true);

	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABackpackActor::Multicast_OnDropped_Implementation(ACharacter* Character)
{
	if (!Character) return;

	Character->SetActorScale3D(OriginalScale);

	if (UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(Character->GetCharacterMovement()))
	{
		MovementComp->MaxWalkSpeed = 600; //trying to find a way to make this work a little better but for now manually setting
	}

	BackpackMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	BackpackMesh->SetSimulatePhysics(true);
	BackpackMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABackpackActor::BackpackPlayerInteraction(ACharacter* InteractingPlayer) 
{
	if (!HasAuthority()) return;

	//inventory logic
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Someone is touching the backpack!"));
}

void ABackpackActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABackpackActor, Inventory);
}

void ABackpackActor::OnRep_Inventory()
{

}
