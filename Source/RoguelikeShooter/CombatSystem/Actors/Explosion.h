#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class ROGUELIKESHOOTER_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	

	AExplosion();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* NiagaraComponent;

	void BeginPlay() override;

	UFUNCTION()
	void OnSystemFinished(class UNiagaraComponent* System);

public:	

	void InitExplosion(AActor* ExplosionOwner, float Damage, float DamageDistance, float Impulse);

};
