#include "BaseProjectile.h"

ABaseProjectile::ABaseProjectile()
{
	bReplicates = true;
}

void ABaseProjectile::InitProjectile(FVector Direction, float Damage, AActor* ShooterActor)
{
	BaseDamage = Damage;
	OwnerActor = ShooterActor;
}

bool ABaseProjectile::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	const APlayerController* controller = Cast<APlayerController>(RealViewer);

	if (controller)
		return (AActor*)controller->GetPawn() != OwnerActor;

	return true;
}

