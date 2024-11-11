#include "PlayerInteractionComponent.h"

#include "../PlayerPawn.h"
#include "Interactable.h"
#include "InteractablePickup.h"

#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>

UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	PlayerPawn = nullptr;
}

void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetOwner());

	if (!PlayerPawn)
		DestroyComponent();

	PlayerPawn->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UPlayerInteractionComponent::OnPawnHit);
}

void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateInteractableInSight();

	HandleInput();
}

void UPlayerInteractionComponent::UpdateInteractableInSight()
{
	const float interactionDistance = 150.0f;
	const FVector startLocation = PlayerPawn->GetCameraComponent()->GetComponentLocation();
	const FVector endLocation = startLocation + PlayerPawn->GetCameraComponent()->GetForwardVector() * interactionDistance;

	FCollisionQueryParams params;
	params.AddIgnoredActor(PlayerPawn);

	FHitResult hit;
	GetWorld()->LineTraceSingleByProfile(hit, startLocation, endLocation, "BlockAll", params);

	if (!hit.bBlockingHit)
	{
		if (InteractableInSight.GetInterface())
		{	
			InteractableInSight->StopHovering();
		}

		InteractableInSight = nullptr;
		return;
	}

	if (hit.GetActor() == InteractableInSight.GetObject())
		return;

	TScriptInterface<class IInteractable> newInteractableInSight = hit.GetActor();

	if (!newInteractableInSight.GetInterface())
		return;

	newInteractableInSight->StartHovering();
	InteractableInSight = newInteractableInSight;
}

void UPlayerInteractionComponent::HandleInput()
{
	FPlayerInput& playerInput = PlayerPawn->GetPlayerInput();

	if (playerInput.bInteract) 
	{
		playerInput.bInteract = false;

		if (!InteractableInSight.GetInterface())
			return;

		InteractableInSight->Interact(PlayerPawn);
	}
}

void UPlayerInteractionComponent::OnPawnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
		return;

	IInteractablePickup* interactablePickup = Cast<IInteractablePickup>(OtherActor);

	if (!interactablePickup)
		return;

	interactablePickup->Pickup(PlayerPawn);
}
