// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePiece.h"

#include "Net/UnrealNetwork.h"


// Sets default values

APuzzlePiece::APuzzlePiece()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollisionComp->SetupAttachment(Mesh);

	HoldableActorComponent = CreateDefaultSubobject<UHoldableActorComponent>("HoldableActorComponent");

	setShouldExecOnServer(true);
	bReplicates = true;
	Holder = nullptr;
}

void APuzzlePiece::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APuzzlePiece, Holder);
}

// Called when the game starts or when spawned

void APuzzlePiece::BeginPlay()
{
	Super::BeginPlay();

	HoldableActorComponent->OnDropDelegate.AddUObject(this, &APuzzlePiece::Dropped);
}

void APuzzlePiece::OnInteract(APawn* Sender)
{
	IInteractiveInterface::OnInteract(Sender);
	UE_LOG(LogTemp, Warning, TEXT("OnInteract Has Authority %s"), HasAuthority() ? TEXT("True") : TEXT("False"));

	if (AEscapeGameCharacter* player = Cast<AEscapeGameCharacter>(Sender))
	{
		UE_LOG(LogTemp, Warning, TEXT("Is locally controleld: %s"), player->IsLocallyControlled() ? TEXT("True") : TEXT("False"));
		UE_LOG(LogTemp, Warning, TEXT("Net Push Id: %d"), player->GetNetPushId());
		
		SetOwner(player);
		Holder = player;
		HolderChanged();
	}
}

void APuzzlePiece::HolderChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("Holder Changed OnRep"));
	
	if (Holder == nullptr) return;

	FAttachmentTransformRules attachRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	attachRules.ScaleRule = EAttachmentRule::KeepWorld;

	if (Holder->IsLocallyControlled())
	{
		AttachToComponent( Holder->GetMesh1P(), attachRules, Socket_Name);
		HoldableActorComponent->PickedUp(Holder);
	} else
	{
		AttachToComponent( Holder->GetMesh(), attachRules, Socket_Name);
	}

	Mesh->SetSimulatePhysics(false); // Disable physique to prevent strange comportement
	SetActorEnableCollision(false);
}

void APuzzlePiece::Dropped()
{
	UE_LOG(LogTemp, Warning, TEXT("Dropped()"));
}
