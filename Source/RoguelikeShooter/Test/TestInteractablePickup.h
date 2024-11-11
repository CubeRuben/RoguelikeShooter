#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Pawns/Components/InteractablePickup.h"
#include "TestInteractablePickup.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API ATestInteractablePickup : public AActor, public IInteractablePickup
{
	GENERATED_BODY()
	
public:	

	ATestInteractablePickup();

	void Pickup(class APlayerPawn* Pawn) override;
};
