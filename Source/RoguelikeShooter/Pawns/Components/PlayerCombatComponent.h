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

	virtual void BeginPlay() override;

	void HandleInput();

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UFirearm* GetCurrentFirearm();
	
	void AddFirearm(class UFirearm* NewFirearm);

	void SetCurrentFirearm(int Index);
};