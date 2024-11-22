#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"


UCLASS()
class ROGUELIKESHOOTER_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerInteractionComponent();

protected:

	class APlayerPawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TScriptInterface<class IInteractable> InteractableInSight;

	virtual void BeginPlay() override;

	void UpdateInteractableInSight();
	void HandleInput();

	UFUNCTION()
	void OnPawnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	void Interact_ServerRPC(AActor* Actor);
	void Interact_ServerRPC_Implementation(AActor* Actor);
};
