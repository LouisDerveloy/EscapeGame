// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    DoorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRoot"));
    SetRootComponent(DoorRoot);

    DoorSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("DoorMesh");
    DoorSkeletalMesh->SetupAttachment(DoorRoot);

	// Initiate variables
	bOpen = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

bool ADoor::ToggleDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleDoor : This actor has %s"), ( HasAuthority() ? TEXT("authority") : TEXT("no authority") ));
	
	bOpen = !bOpen;

	if (HasAuthority())
	{
		// if bOpen changed to true play opening animation if it changed to false play closing animation.
		DoorSkeletalMesh->PlayAnimation(bOpen ? DoorOpening_Animation : DoorClosing_Animation, false);
	}
	
	return bOpen;
}

void ADoor::OnRep_bOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_bOpen Has authority : %s"), (HasAuthority() ? TEXT("yes") : TEXT("no")));
	UE_LOG(LogTemp, Warning, TEXT("bOpen : %s"), (bOpen ? TEXT("yes") : TEXT("no")));

	// if bOpen changed to true play opening animation if it changed to false play closing animation.
	DoorSkeletalMesh->PlayAnimation(bOpen ? DoorOpening_Animation : DoorClosing_Animation, false);
}

void ADoor::OnInteract(APawn* Sender)
{
	UE_LOG(LogTemp, Warning, TEXT("Server : ServerInteract"));
	
	ToggleDoor();
}

void ADoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bOpen);
}