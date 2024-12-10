#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../CombatSystem/AmmoContainer.h"
#include "PlayerCombatComponent.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadUpdateDelegate);

UCLASS(BlueprintType)
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
	bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ReloadingTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<class UFirearm>> HeldFirearms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<FAmmoContainer> ContainedAmmo;

	// Delegates

	UPROPERTY(BlueprintAssignable)
	FReloadUpdateDelegate OnReloadStart;

	UPROPERTY(BlueprintAssignable)
	FReloadUpdateDelegate OnReloadStop;

	virtual void BeginPlay() override;

	void HandleInput();
	void UpdateReloading(float DeltaTime);

	void StartReloading();
	void StopReloading();

	void DropFirearm(UFirearm* Firearm);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UFirearm* GetCurrentFirearm();
	
	FVector GetAimAdjustDirection(FVector Offset);

	void AddFirearm(class UFirearm* NewFirearm);

	void SetCurrentFirearm(int Index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetReloadTimerRatio();

	bool AddAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);
	int GetAmmoAmount(class UAmmoDefinition* AmmoDefinition) const;
	bool CanConsumeAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);
	bool ConsumeAmmo(class UAmmoDefinition* AmmoDefinition, int AmmoAmount);

	bool ReplicateSubobjects(class UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION(Server, Reliable)
	void Fire_ServerRPC();
	void Fire_ServerRPC_Implementation();

	UFUNCTION(Server, Reliable)
	void ReloadAmmo_ServerRPC();
	void ReloadAmmo_ServerRPC_Implementation();

	UFUNCTION(Server, Reliable)
	void SetFireMode_ServerRPC(bool bFireMode);
	void SetFireMode_ServerRPC_Implementation(bool bFireMode);

};
