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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Acceleration;

public:

	void Tick(float DeltaTime) override;

	void InitProjectile(FVector Direction, float Damage, AActor* ShooterActor) override;
};
