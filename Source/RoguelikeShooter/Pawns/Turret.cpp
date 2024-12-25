#include "Turret.h"

#include "PlayerPawn.h"
#include "../CombatSystem/Projectiles/BaseProjectile.h"

#include <NiagaraComponent.h>
#include <NiagaraSystem.h>
#include <NiagaraFunctionLibrary.h>

#include <Components/PoseableMeshComponent.h>
#include <Components/SphereComponent.h>

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates	= true;
	SetReplicateMovement(true);

	TurretMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Turret Mesh Component"));
	TurretMeshComponent->SetSimulatePhysics(true);

	TriggerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere Component"));
	TriggerSphereComponent->InitSphereRadius(2500.0f);
	TriggerSphereComponent->SetCollisionProfileName("Trigger");
	TriggerSphereComponent->SetGenerateOverlapEvents(true);
	TriggerSphereComponent->SetupAttachment(TurretMeshComponent);

	LaserNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Laser Niagara Component"));
	LaserNiagaraComponent->SetupAttachment(TurretMeshComponent, "laserSocket");

	BodyRotationSpeed = 35.0f;
	BodyRotationAttackingSpeed = 50.f;
	BodyMaxRotationAngle = 75.0f;
	BodyRotationDirection = 1.0f;

	TargetBodyRotation = 0.0f;

	AIState = ETurretState::LookingAround;

	AttackType = ETurretAttackType::Hitscan;

	AttackCooldown = 0.25f;
	AttackCooldownTimer = 0.0f;
	Damage = 15.0f;
	HitscanScatterAngle = 5.0f;

	HealthPoints = 200.0f;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnTriggerBeginOverlap);
	TriggerSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnTriggerEndOverlap);

	BodyMaxAngleCos = FMath::Cos(FMath::DegreesToRadians(BodyMaxRotationAngle));
}

void ATurret::UpdateLookingAround(float DeltaTime)
{
	APlayerPawn* closetsTarget = nullptr;

	for (TWeakObjectPtr<APlayerPawn>& pawn : TargetedPlayers) 
	{
		if (!pawn.IsValid())
			continue;

		if (!IsValidTarget(pawn.Get()))
			continue;

		if (!closetsTarget) 
		{
			closetsTarget = pawn.Get();
			continue;
		}

		if (FVector::DistSquared(GetActorLocation(), closetsTarget->GetActorLocation()) > FVector::DistSquared(GetActorLocation(), pawn->GetActorLocation()))
			closetsTarget = pawn.Get();
	}

	if (closetsTarget) 
	{
		TargetActor = closetsTarget;
		AIState = ETurretState::Attacking;
		return;
	}

	if (FMath::Abs(TargetBodyRotation) >= BodyMaxRotationAngle)
		BodyRotationDirection = -BodyRotationDirection;

	TargetBodyRotation = FMath::Clamp(TargetBodyRotation + BodyRotationDirection * BodyRotationSpeed * DeltaTime, -BodyMaxRotationAngle, BodyMaxRotationAngle);

	const FVector startLocation = LaserNiagaraComponent->GetComponentLocation();
	const FVector endLocation = startLocation + LaserNiagaraComponent->GetForwardVector() * 2500.f;

	FVector laserEndLocation = endLocation;
	
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility, params))
		laserEndLocation = hitResult.ImpactPoint;
	
	LaserNiagaraComponent->SetVectorParameter("LaserEnd", laserEndLocation);
}

void ATurret::UpdateAttackTarget(float DeltaTime)
{
	if (!TargetActor.IsValid() || !IsValidTarget(TargetActor.Get()))
	{
		TargetActor = nullptr;
		AIState = ETurretState::LookingAround;
		return;
	}

	const FVector targetVector = FVector::VectorPlaneProject(TargetActor->GetActorLocation() - GetActorLocation(), GetActorUpVector());
	const FVector targetDirection = targetVector.GetSafeNormal();
	const FQuat quat = FQuat::FindBetweenNormals(GetActorForwardVector(), targetDirection);
	const float targetRotation = quat.Euler().Z;

	const float deltaRotation = targetRotation - TargetBodyRotation;
	const float deltaSpeedRotation = FMath::Sign(deltaRotation) * BodyRotationAttackingSpeed * DeltaTime;
	const float clampedDeltaSpeedRotation = FMath::Sign(deltaRotation) * FMath::Min(FMath::Abs(deltaSpeedRotation), FMath::Abs(deltaRotation));

	TargetBodyRotation = FMath::Clamp(TargetBodyRotation + clampedDeltaSpeedRotation, -BodyMaxRotationAngle, BodyMaxRotationAngle);

	LaserNiagaraComponent->SetVectorParameter("LaserEnd", TargetActor->GetActorLocation() + FVector(0.0f, 0.0f, 25.0f));

	if (FVector::DotProduct(LaserNiagaraComponent->GetForwardVector(), (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal()) < 0.8f)
		return;

	if (AttackCooldownTimer > 0.0f)
		return;

	AttackCooldownTimer = AttackCooldown;

	if (AttackType == ETurretAttackType::Hitscan) 
	{
		const FVector startPosition = TurretMeshComponent->GetSocketLocation("gunheadSocket");

		const float xRotationOffset = FMath::FRandRange(-HitscanScatterAngle, HitscanScatterAngle);
		const float yRotationOffset = FMath::FRandRange(-HitscanScatterAngle, HitscanScatterAngle);
		const float zRotationOffset = FMath::FRandRange(-HitscanScatterAngle, HitscanScatterAngle);

		const FVector rotatedShootingDirection = LaserNiagaraComponent->GetForwardVector()
			.RotateAngleAxis(xRotationOffset, FVector::ForwardVector)
			.RotateAngleAxis(yRotationOffset, FVector::RightVector)
			.RotateAngleAxis(zRotationOffset, FVector::UpVector);

		const FVector endPosition = startPosition + rotatedShootingDirection * 2500.0f;

		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		FHitResult hitResult;
		if (!GetWorld()->LineTraceSingleByProfile(hitResult, startPosition, endPosition, "BlockAll", params))
		{
			SpawnVisual(startPosition, endPosition);
			return;
		}

		SpawnVisual(startPosition, hitResult.Location);

		if (!hitResult.GetActor())
			return;

		IDamageable* damageable = Cast<IDamageable>(hitResult.GetActor());

		if (!damageable)
			return;

		FDamageParams damageParams;
		damageParams.DamageSource = this;
		damageParams.HitDirection = LaserNiagaraComponent->GetForwardVector();
		damageParams.HitLocation = hitResult.Location;

		damageable->ApplyDamage(Damage, &damageParams);
	}
	else if (AttackType == ETurretAttackType::Projectile)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABaseProjectile* projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, TurretMeshComponent->GetSocketLocation("gunheadSocket"), FRotator::ZeroRotator, params);
		const float xRotationOffset = FMath::FRandRange(-HitscanScatterAngle, HitscanScatterAngle);
		const float yRotationOffset = FMath::FRandRange(-HitscanScatterAngle, HitscanScatterAngle);
		const float zRotationOffset = FMath::FRandRange(-HitscanScatterAngle, HitscanScatterAngle);
		const FVector rotatedShootingDirection = LaserNiagaraComponent->GetForwardVector()
			.RotateAngleAxis(xRotationOffset, FVector::ForwardVector)
			.RotateAngleAxis(yRotationOffset, FVector::RightVector)
			.RotateAngleAxis(zRotationOffset, FVector::UpVector);
		projectile->InitProjectile(rotatedShootingDirection, Damage, this);
	}
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackCooldownTimer > 0.0f)
		AttackCooldownTimer -= DeltaTime;

	switch (AIState)
	{
	case ETurretState::LookingAround:
		UpdateLookingAround(DeltaTime);
		break;
	case ETurretState::Attacking:
		UpdateAttackTarget(DeltaTime);
		break;
	default:
		break;
	}
}

void ATurret::ApplyDamage(float DamageAmount, FDamageParams* DamageParams)
{
	HealthPoints -= DamageAmount;

	if (HealthPoints <= 0)
		Destroy();
}

void ATurret::ApplyImpulse(FVector Impulse)
{
	TurretMeshComponent->AddImpulse(Impulse);
}

void ATurret::SpawnVisual(FVector StartLocation, FVector EndLocation)
{
	if (!TraceParticleSystem)
		return;

	UNiagaraComponent* niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, TraceParticleSystem, StartLocation);

	if (!niagaraComponent)
		return;

	niagaraComponent->SetVectorParameter(TEXT("TraceEnd"), EndLocation - StartLocation);
}

void ATurret::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerPawn* playerPawn = Cast<APlayerPawn>(OtherActor);
	
	if (playerPawn)
		TargetedPlayers.Add(playerPawn);
}

void ATurret::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerPawn* playerPawn = Cast<APlayerPawn>(OtherActor);

	if (playerPawn)
	{
		TargetedPlayers.Remove(playerPawn);

		if (TargetActor == playerPawn)
		{
			TargetActor = nullptr;
			AIState = ETurretState::LookingAround;
			return;
		}

	}
}

float ATurret::TargetDotProduct(const FVector& Location)
{
	const FVector targetDirection = (Location - GetActorLocation()).GetSafeNormal();
	return FVector::DotProduct(GetActorForwardVector(), targetDirection);
}

bool ATurret::IsTargetInRange(AActor* Target)
{
	return BodyMaxAngleCos < TargetDotProduct(Target->GetActorLocation());
}

bool ATurret::IsTargetVisible(AActor* Target)
{
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(Target);

	FHitResult hitResult;

	bool detected = false;
	detected |= !GetWorld()->LineTraceSingleByChannel(hitResult, LaserNiagaraComponent->GetComponentLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_Visibility, params);
	detected |= !GetWorld()->LineTraceSingleByChannel(hitResult, LaserNiagaraComponent->GetComponentLocation(), Target->GetActorLocation() + FVector(0.0f, 0.0f, 70.0f), ECollisionChannel::ECC_Visibility, params);
	detected |= !GetWorld()->LineTraceSingleByChannel(hitResult, LaserNiagaraComponent->GetComponentLocation(), Target->GetActorLocation() - FVector(0.0f, 0.0f, 70.0f), ECollisionChannel::ECC_Visibility, params);
	detected |= !GetWorld()->LineTraceSingleByChannel(hitResult, LaserNiagaraComponent->GetComponentLocation(), Target->GetActorLocation() + FVector(30.0f, 0.0f, 0.0f), ECollisionChannel::ECC_Visibility, params);
	detected |= !GetWorld()->LineTraceSingleByChannel(hitResult, LaserNiagaraComponent->GetComponentLocation(), Target->GetActorLocation() + FVector(0.0f, 30.0f, 0.0f), ECollisionChannel::ECC_Visibility, params);
	detected |= !GetWorld()->LineTraceSingleByChannel(hitResult, LaserNiagaraComponent->GetComponentLocation(), Target->GetActorLocation() - FVector(30.0f, 0.0f, 0.0f), ECollisionChannel::ECC_Visibility, params);
	detected |= !GetWorld()->LineTraceSingleByChannel(hitResult, LaserNiagaraComponent->GetComponentLocation(), Target->GetActorLocation() - FVector(0.0f, 30.0f, 0.0f), ECollisionChannel::ECC_Visibility, params);
	return detected;
}

bool ATurret::IsValidTarget(AActor* Target)
{
	return IsTargetInRange(Target) && IsTargetVisible(Target);
}
