#include "PlayerPawn.h"

#include "Components/PlayerMovementComponent.h"
#include "Components/PlayerHealthComponent.h"
#include "Components/PlayerInteractionComponent.h"
#include "Components/PlayerCombatComponent.h"

#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Camera/CameraComponent.h>

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->InitCapsuleSize(25.0f, 75.0f);
	CapsuleComponent->SetCollisionProfileName("Player");
	RootComponent = CapsuleComponent;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera Component"));
	PlayerCamera->SetupAttachment(CapsuleComponent);
	PlayerCamera->AddLocalOffset(FVector(0.0f, 0.0f, 65.0f));

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Skeletal Mesh Component"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	MovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("Movement Component"));
	HealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("Health Component"));
	InteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("Interaction Component"));
	CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("Combat Component"));
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled()) 
	{
		SkeletalMeshComponent->SetHiddenInGame(true);
	}
}

void APlayerPawn::ApplyDamage(float DamageAmount, FDamageParams* DamageParams)
{
	AActor* damageSource = nullptr;

	if (DamageParams)
		damageSource = DamageParams->DamageSource;

	HealthComponent->ApplyDamage(DamageAmount, damageSource);
}

void APlayerPawn::ApplyImpulse(FVector Impulse)
{
	MovementComponent->Velocity += Impulse / 100.0f;

	if (HasAuthority())
		ApplyImpulse_ClientRPC(Impulse);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APlayerPawn::GetClientLocation() const
{
	if (IsLocallyControlled())
		return GetActorLocation();

	return MovementComponent->GetReplicatedLocation();
}

FRotator APlayerPawn::GetClientRotation() const
{
	if (IsLocallyControlled())
		return GetActorRotation();

	return FRotator(0.0f, MovementComponent->GetReplicatedRotation(), 0.0f);
}

FVector APlayerPawn::GetClientCameraLocation() const
{
	if (IsLocallyControlled())
		return PlayerCamera->GetComponentLocation();

	return MovementComponent->GetReplicatedLocation() + PlayerCamera->GetRelativeLocation();
}

FRotator APlayerPawn::GetClientCameraRotation() const
{
	if (IsLocallyControlled())
		return PlayerCamera->GetComponentRotation();

	return FRotator(0.0f, MovementComponent->GetReplicatedRotation(), 0.0f) + PlayerCamera->GetRelativeRotation();
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
		return;

	PlayerInputComponent->BindAxis("MouseX", this, &APlayerPawn::InputMouseXAxis);
	PlayerInputComponent->BindAxis("MouseY", this, &APlayerPawn::InputMouseYAxis);
	PlayerInputComponent->BindAxis("MouseWheel", this, &APlayerPawn::InputMouseWheelAxis);

#define INPUT_BIND(name) PlayerInputComponent->BindAction(#name, IE_Pressed, this, &APlayerPawn::Input##name##Pressed); PlayerInputComponent->BindAction(#name, IE_Released, this, &APlayerPawn::Input##name##Released);

	INPUT_BIND(MoveForward);
	INPUT_BIND(MoveLeft);
	INPUT_BIND(MoveBack);
	INPUT_BIND(MoveRight);

	INPUT_BIND(Interact);

	INPUT_BIND(Run);
	INPUT_BIND(Jump);

	INPUT_BIND(FireWeapon);
	INPUT_BIND(AlternativeWeaponAction);
	INPUT_BIND(SwitchFireMode);
	INPUT_BIND(Reload);
	INPUT_BIND(DropWeapon);
}

void APlayerPawn::ApplyImpulse_ClientRPC_Implementation(FVector Impulse)
{
	MovementComponent->Velocity += Impulse / 100.0f;
}
