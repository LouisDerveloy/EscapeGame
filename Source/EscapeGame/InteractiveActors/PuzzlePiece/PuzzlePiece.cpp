// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePiece.h"


// Sets default values
APuzzlePiece::APuzzlePiece()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollisionComp->SetupAttachment(Mesh);

	
	PrimaryActorTick.bCanEverTick = false;
	setShouldExecOnServer(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void APuzzlePiece::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzlePiece::OnInteract(APawn* Sender)
{
	IInteractiveInterface::OnInteract(Sender);
	UE_LOG(LogTemp, Warning, TEXT("OnInteract Has Authority %s"), HasAuthority() ? TEXT("True") : TEXT("False"));

	if (AEscapeGameCharacter* player = Cast<AEscapeGameCharacter>(Sender))
	{
		SetOwner(player);

		PickedUpBy(player);
	}
}

void APuzzlePiece::PickedUpBy_Implementation(AEscapeGameCharacter* player)
{
	FAttachmentTransformRules attachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	attachRules.ScaleRule = EAttachmentRule::KeepWorld;

	// The local player only see the arms but the other player see the whole body so we need to adapt wich mesh we use.

	UE_LOG(LogTemp, Warning, TEXT("Is locally controleld: %s"), player->IsLocallyControlled() ? TEXT("True") : TEXT("False"));
	UE_LOG(LogTemp, Warning, TEXT("Is locally controleld: %d"), player->GetNetPushId());
	
	if (player->IsLocallyControlled())
	{
		AttachToComponent( player->GetMesh1P(), attachRules, Socket_Name);
	}
	else
	{
		AttachToComponent( player->GetMesh(), attachRules, Socket_Name);
	}
	Mesh->SetSimulatePhysics(false); // Disable physique to prevent strange comportement
	SetActorEnableCollision(false);
}