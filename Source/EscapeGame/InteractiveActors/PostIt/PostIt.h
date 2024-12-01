// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputAction.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "../../EscapeGameCharacter.h"

#include "../InteractiveInterface.h"
#include "Components/ArrowComponent.h"

#include "PostIt.generated.h"

UCLASS()
class ESCAPEGAME_API APostIt : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category=PostIt)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, Category=PostIt)
	UArrowComponent* InspectCameraLocation;

	UPROPERTY(EditAnywhere, Category=PostIt)
	UInputAction* UnpossessAction;

	UPROPERTY(EditAnywhere, Category=PostIt)
	UInputMappingContext* InspectionMappingContext;

	UPROPERTY(EditAnywhere, Category=PostIt)
	UInputMappingContext* DefaultMappingContext;

public:
	// Sets default values for this pawn's properties
	APostIt();

	UFUNCTION()
	void StopInspection();

	UFUNCTION()
	void StartInspection(APawn* Sender);

	UFUNCTION()
	virtual void OnInteract(APawn* Sender) override;

protected:
	UPROPERTY()
	AEscapeGameCharacter* Inspector = nullptr;
	
	FTransform DefaultPlayerCameraTransform;

};
