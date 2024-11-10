#include "PlayerPawn.h"

#include "Components/PlayerMovementComponent.h"
#include "Components/PlayerHealthComponent.h"

#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->InitCapsuleSize(25.0f, 75.0f);
	CapsuleComponent->SetCollisionProfileName("BlockAll");
	CapsuleComponent->SetHiddenInGame(false);
	RootComponent = CapsuleComponent;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera Component"));
	PlayerCamera->SetupAttachment(CapsuleComponent);
	PlayerCamera->AddLocalOffset(FVector(0.0f, 0.0f, 65.0f));

	MovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("Movement Component"));
	HealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("Health Component"));
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Called");

	PlayerInputComponent->BindAxis("MouseX", this, &APlayerPawn::InputMouseXAxis);
	PlayerInputComponent->BindAxis("MouseY", this, &APlayerPawn::InputMouseYAxis);

#define INPUT_BIND(name) PlayerInputComponent->BindAction(#name, IE_Pressed, this, &APlayerPawn::Input##name##Pressed); PlayerInputComponent->BindAction(#name, IE_Released, this, &APlayerPawn::Input##name##Released);

	INPUT_BIND(MoveForward);
	INPUT_BIND(MoveLeft);
	INPUT_BIND(MoveBack);
	INPUT_BIND(MoveRight);

	INPUT_BIND(Run);
	INPUT_BIND(Jump);
}

