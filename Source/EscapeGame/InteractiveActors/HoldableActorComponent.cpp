// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldableActorComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values for this component's properties
UHoldableActorComponent::UHoldableActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHoldableActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHoldableActorComponent::Drop()
{
	OnDropDelegate.Broadcast();
}

void UHoldableActorComponent::PickedUp(AEscapeGameCharacter* player)
{
	if (player == nullptr) return;

	APlayerController* controller = Cast<APlayerController>(player->GetController());
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer()))
	{
		subsystem->AddMappingContext(MappingContext, 1);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(player->InputComponent))
	{
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &UHoldableActorComponent::Drop);
	}
}
