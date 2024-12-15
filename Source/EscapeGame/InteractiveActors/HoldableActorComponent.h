// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EscapeGame/EscapeGameCharacter.h"
#include "HoldableActorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDropDelegate)

/*
 * This class is used to add the comportment needed to handle the input related to holdable actor.
 * So for each actor that can be held ( owned ) by a player you can use this component to add the wanted mapping context.
 *
 * When the actor is PickedUp, you have to call the PickedUp function of the component.
 * -> This function add the Mapping Context specified in the detail panel.
 * -> It also bind the Drop Input Action which when it's pressed will call the OnDrop delegate. And also Unbind and Remove all the mapping context and action input added from this component.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ESCAPEGAME_API UHoldableActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHoldableActorComponent();

	void PickedUp(AEscapeGameCharacter* player);

	FOnDropDelegate OnDropDelegate;

	UPROPERTY(EditAnywhere, Category="HoldableActor|Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category="HoldableActor|Input")
	UInputAction* DropAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Drop();
};
