#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseProjectile();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* OwnerActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage;

public:

	virtual void InitProjectile(FVector Direction, float Damage, AActor* ShooterActor);

};
