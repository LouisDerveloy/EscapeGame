// Fill out your copyright notice in the Description page of Project Settings.


#include "Postit.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystemTypes.h"
#include "Camera/CameraComponent.h"
#include "EscapeGame/EscapeGamePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APostIt::APostIt()
{

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComp->SetupAttachment(Root);

	InspectCameraLocation = CreateDefaultSubobject<UArrowComponent>("InspectCameraLocation");
	InspectCameraLocation->SetupAttachment(Root);
	
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	DefaultPlayerCameraTransform = FTransform::Identity;
	setShouldExecOnServer(false); // IInteractiveInterface for interaction actor
}

void APostIt::StopInspection()
{
	UE_LOG(LogTemp, Warning, TEXT("APostIt::StopInspection"));

	if (Inspector)
	{
		// Reset player camera relative location
		Inspector->GetFirstPersonCameraComponent()->bUsePawnControlRotation = true;
		Inspector->GetFirstPersonCameraComponent()->SetRelativeTransform(DefaultPlayerCameraTransform);

		// Remove binding to the unpossess action and restore bindind for the character
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent Casted unbind"));
		
		AEscapeGamePlayerController* playerController = Cast<AEscapeGamePlayerController>(Inspector->Controller);
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->ClearAllMappings();
			subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APostIt::StartInspection(APawn* Sender)
{
	UE_LOG(LogTemp, Warning, TEXT("APostIt::StartInspection"));
	
	if (AEscapeGameCharacter* Character = Cast<AEscapeGameCharacter>(Sender))
	{
		Inspector = Character;
		
		// Move the player camera
		Character->GetFirstPersonCameraComponent()->bUsePawnControlRotation = false;
		DefaultPlayerCameraTransform = Character->GetFirstPersonCameraComponent()->GetRelativeTransform();
		Character->GetFirstPersonCameraComponent()->SetWorldTransform(InspectCameraLocation->GetComponentTransform());

		// Setup new input and remove input for the character
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Character->Controller->InputComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent Casted bind"));

			APlayerController* playerController = Cast<APlayerController>(Character->Controller);
			if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
			{
				subsystem->ClearAllMappings();
				subsystem->AddMappingContext(InspectionMappingContext, 1); // Priority is 1 to consume Interact action
			}
			EnhancedInputComponent->BindAction(UnpossessAction, ETriggerEvent::Triggered, this, &APostIt::StopInspection);
		}
	}
}

void APostIt::OnInteract(APawn* Sender)
{
	UE_LOG(LogTemp, Warning, TEXT("APostIt::OnInteract"));

	StartInspection(Sender);
}
