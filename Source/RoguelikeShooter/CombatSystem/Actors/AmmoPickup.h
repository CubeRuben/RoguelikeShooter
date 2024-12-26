#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Pawns/Components/InteractablePickup.h"
#include "../Damageable.h"
#include "AmmoPickup.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API AAmmoPickup : public AActor, public IInteractablePickup, public IDamageable
{
	GENERATED_BODY()
	
public:	

	AAmmoPickup();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_AmmoDefinition)
	class UAmmoDefinition* AmmoDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoAmount;

	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_AmmoDefinition();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	void Pickup(class APlayerPawn* Pawn) override;
	void ApplyImpulse(FVector Impulse) override;
	FVector GetLocation() override { return GetActorLocation(); }
};
