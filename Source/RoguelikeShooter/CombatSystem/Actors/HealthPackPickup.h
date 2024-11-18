#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Pawns/Components/InteractablePickup.h"
#include "../Damageable.h"
#include "HealthPackPickup.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API AHealthPackPickup : public AActor, public IInteractablePickup, public IDamageable
{
	GENERATED_BODY()
	
public:	

	AHealthPackPickup();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPToHeal;

public:

	void Pickup(class APlayerPawn* Pawn) override;
	void ApplyImpulse(FVector Impulse) override;

};
