// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "../InteractiveInterface.h"
#include "Door.generated.h"

UCLASS()
class ESCAPEGAME_API ADoor : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();
	
	// Handle interaction
	virtual void OnInteract(APawn* Sender) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	bool ToggleDoor();

	UFUNCTION()
	void OnRep_bOpen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Door)
	UAnimSequence* DoorOpening_Animation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Door)
	UAnimSequence* DoorClosing_Animation;

	UPROPERTY(ReplicatedUsing=OnRep_bOpen ,EditAnywhere, BlueprintReadOnly, Category = Door)
	bool bOpen;



public:
	UPROPERTY(EditAnywhere, category = Door)
	USkeletalMeshComponent* DoorSkeletalMesh;

	UPROPERTY(EditAnywhere, category = Door)
	USceneComponent* DoorRoot;
	
};
