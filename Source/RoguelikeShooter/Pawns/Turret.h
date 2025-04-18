#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../CombatSystem/Damageable.h"
#include "Turret.generated.h"

UENUM(BlueprintType)
enum class ETurretState : uint8 
{
	LookingAround,
	Attacking
};

UENUM(BlueprintType)
enum class ETurretAttackType : uint8
{
	Hitscan,
	Projectile
};

UCLASS(BlueprintType)
class ROGUELIKESHOOTER_API ATurret : public APawn, public IDamageable
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HealthPoints;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float TargetBodyRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FVector LaserEnd_Replicated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETurretState AIState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETurretAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HitscanScatterAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraSystem* TraceParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABaseProjectile> ProjectileClass;

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

	void SpawnVisual(FVector StartLocation, FVector EndLocation);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnShot();

public:	

	virtual void Tick(float DeltaTime) override;

	void ApplyDamage(float DamageAmount, FDamageParams* DamageParams = nullptr) override;
	void ApplyImpulse(FVector Impulse) override;
	FVector GetLocation() override { return GetActorLocation() + FVector(0.0f, 0.0f, 100.0f); }

	UFUNCTION(NetMulticast, Reliable)
	void SpawnVisual_MulticastRPC(FVector StartLocation, FVector EndLocation);
	void SpawnVisual_MulticastRPC_Implementation(FVector StartLocation, FVector EndLocation);

	UFUNCTION(NetMulticast, Reliable)
	void OnShot_MulticastRPC();
	void OnShot_MulticastRPC_Implementation();
};
