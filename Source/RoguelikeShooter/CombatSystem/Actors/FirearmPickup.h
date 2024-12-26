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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent* FirearmWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFirearmDescriptionWidget* FirearmWidget;

	UPROPERTY(EditAnywhere, Instanced, ReplicatedUsing=InitPickup)
	TObjectPtr<class UFirearm> Firearm;

	virtual void BeginPlay() override;

	UFUNCTION()
	void InitPickup();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	

	UFUNCTION(BlueprintCallable)
	void SetFirearm(UFirearm* NewFirearm);

	virtual void Tick(float DeltaTime) override;

	void Interact(class APlayerPawn* Pawn) override;

	void StartHovering() override;
	void StopHovering() override;

	bool ReplicateSubobjects(class UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
};
