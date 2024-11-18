#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCombatComponent.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API UPlayerCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPlayerCombatComponent();

protected:
	
	class APlayerPawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentFirearmIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class UFirearm>> HeldFirearms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<class UAmmoDefinition*, int> ContainedAmmo;


	virtual void BeginPlay() override;

	void HandleInput();

	

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UFirearm* GetCurrentFirearm();
	
	FVector GetAimAdjustDirection(FVector Offset);

	void AddFirearm(class UFirearm* NewFirearm);

	void SetCurrentFirearm(int Index);

	bool AddAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);
	bool CanConsumeAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);
	bool ConsumeAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);
};
