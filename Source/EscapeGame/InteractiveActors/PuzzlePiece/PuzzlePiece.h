// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "EscapeGame/EscapeGameCharacter.h"
#include "EscapeGame/InteractiveActors/InteractiveInterface.h"
#include "EscapeGame/InteractiveActors/HoldableActorComponent.h"
#include "GameFramework/Actor.h"
#include "PuzzlePiece.generated.h"

UCLASS()
class ESCAPEGAME_API APuzzlePiece : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=PuzzlePiece)
	USphereComponent* SphereCollisionComp;
	
	UPROPERTY(EditAnywhere, Category=PuzzlePiece)
	UStaticMeshComponent* Mesh;


public:
	// Sets default values for this actor's properties
	APuzzlePiece();

	UPROPERTY(EditAnywhere, Category = PuzzlePiece)
	TObjectPtr<UHoldableActorComponent> HoldableActorComponent;
	
	UFUNCTION()
	virtual void OnInteract(APawn* Sender) override;

	UFUNCTION()
	void HolderChanged();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Dropped();

	UPROPERTY(EditAnywhere, Category=PuzzlePiece)
	FName Socket_Name;

	UPROPERTY(ReplicatedUsing=HolderChanged ,VisibleAnywhere, Category=PuzzlePiece)
	AEscapeGameCharacter* Holder;
};
