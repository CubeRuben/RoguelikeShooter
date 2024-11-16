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

public:	

	virtual void InitProjectile(FVector Direction, float Damage, AActor* OwnerActor);

};
