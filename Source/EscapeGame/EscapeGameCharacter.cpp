// Copyright Epic Games, Inc. All Rights Reserved.

#include "EscapeGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EscapeGamePlayerController.h"
#include "InputActionValue.h"
#include "InteractiveActors/InteractiveInterface.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEscapeGameCharacter

AEscapeGameCharacter::AEscapeGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Initialize properties
	bReplicates = true;
	InteractDistance = 250;

}

void AEscapeGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void AEscapeGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEscapeGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEscapeGameCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AEscapeGameCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEscapeGameCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("AEscapeGameCharacter::Interact"));


	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndLocation = FirstPersonCameraComponent->GetForwardVector() * InteractDistance + StartLocation;
	FHitResult Hit;
	
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 5, 0, 5);

	if (!Hit.bBlockingHit) { return; }
	
	if (Hit.GetActor()->Implements<UInteractiveInterface>())
	{
		TScriptInterface<IInteractiveInterface> InteractiveActor = TScriptInterface<IInteractiveInterface>(Hit.GetActor()); 
		
		UE_LOG(LogTemp, Warning, TEXT("Net Rep Responsible Owner: %s"), (HasNetOwner() ? TEXT("Yes") : TEXT("No")));
		if (InteractiveActor->ShouldExecOnServer())
		{
			ServerInteract(InteractiveActor);
		} else
		{
			InteractiveActor->OnInteract(this);
		}
		
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitted something"));
		
	}
	
}

void AEscapeGameCharacter::ServerInteract_Implementation(const TScriptInterface<IInteractiveInterface>& Target)
{
	if (Target)
	{
		Target.GetInterface()->OnInteract(this);
	}
}


void AEscapeGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AEscapeGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}