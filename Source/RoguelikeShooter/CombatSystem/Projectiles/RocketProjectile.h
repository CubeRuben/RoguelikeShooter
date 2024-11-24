#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "RocketProjectile.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API ARocketProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:

	ARocketProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseExplosionImpulse;

	UPROPERTY(VisibleAnywhere, Replicated)
	FVector Location_Replicated;

	void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	void Tick(float DeltaTime) override;

	void InitProjectile(FVector Direction, float Damage, AActor* ShooterActor) override;
};
