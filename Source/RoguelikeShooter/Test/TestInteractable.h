#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Pawns/Components/Interactable.h"
#include "TestInteractable.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API ATestInteractable : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	ATestInteractable();

protected:

	virtual void BeginPlay() override;


public:	

	// Interactable Interface

	void Interact(class APlayerPawn* Pawn) override;

	void StartHovering() override;
	void StopHovering() override;


};
