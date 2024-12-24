#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

UENUM(BlueprintType)
enum class ETurretState : uint8 
{
	LookingAround,
	Attacking
};

UCLASS(BlueprintType)
class ROGUELIKESHOOTER_API ATurret : public APawn
{
	GENERATED_BODY()

public:

	ATurret();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* TurretMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* TriggerSphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* LaserNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TSet<TWeakObjectPtr<class APlayerPawn>> TargetedPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BodyRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BodyRotationAttackingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BodyMaxRotationAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BodyRotationDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetBodyRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETurretState AIState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> TargetActor;

	float BodyMaxAngleCos;

	virtual void BeginPlay() override;

	void UpdateLookingAround(float DeltaTime);

	void UpdateAttackTarget(float DeltaTime);

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	float TargetDotProduct(const FVector& Location);
	bool IsTargetInRange(AActor* Target);
	bool IsTargetVisible(AActor* Target);
	bool IsValidTarget(AActor* Target);

public:	

	virtual void Tick(float DeltaTime) override;

};
