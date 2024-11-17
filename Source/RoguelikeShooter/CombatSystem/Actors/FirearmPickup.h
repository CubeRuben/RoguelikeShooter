#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Pawns/Components/Interactable.h"
#include "FirearmPickup.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API AFirearmPickup : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	AFirearmPickup();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* FirearmMeshComponent;

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<class UFirearm> Firearm;

	virtual void BeginPlay() override;

	void InitPickup();

public:	

	virtual void Tick(float DeltaTime) override;

	void Interact(class APlayerPawn* Pawn) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
};
