#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "BombProjectile.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API ABombProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:

	ABombProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseExplosionImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Lifetime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetonationTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bEverHitSurface;

	void BeginPlay() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SpawnExplosion();

public:

	void InitProjectile(FVector Direction, float Damage, AActor* ShooterActor) override;

	void Tick(float DeltaTime) override;
};
