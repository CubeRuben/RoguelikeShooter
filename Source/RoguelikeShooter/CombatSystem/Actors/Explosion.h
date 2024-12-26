#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS(BlueprintType)
class ROGUELIKESHOOTER_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	

	AExplosion();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USoundBase* ExplosionSound;

	void BeginPlay() override;

	UFUNCTION()
	void OnSystemFinished(class UNiagaraComponent* System);

public:	
	
	UFUNCTION(BlueprintCallable)
	void InitExplosion(AActor* ExplosionOwner, float Damage, float DamageDistance, float Impulse);

};
