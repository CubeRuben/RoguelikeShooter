#include "FirearmPickup.h"

#include "../Firearm.h"
#include "../FirearmDefinition.h"
#include "../../Pawns/PlayerPawn.h"
#include "../../Pawns/Components/PlayerCombatComponent.h"
#include "../../UI/FirearmDescriptionWidget.h"

#include <Components/WidgetComponent.h>
#include <Net/UnrealNetwork.h>
#include <Engine/ActorChannel.h>

AFirearmPickup::AFirearmPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	FirearmMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Firearm Mesh Component"));
	FirearmMeshComponent->SetSimulatePhysics(true);
	FirearmMeshComponent->SetUseCCD(true);
	RootComponent = FirearmMeshComponent;

	FirearmWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Firearm Widget Component"));
	FirearmWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	FirearmWidgetComponent->SetDrawAtDesiredSize(true);
	FirearmWidgetComponent->SetupAttachment(RootComponent);
	FirearmWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

	ConstructorHelpers::FClassFinder<UFirearmDescriptionWidget> FirearmWidgetClass(TEXT("/Game/UI/UW_FirearmDescriptionWorld"));

	if (FirearmWidgetClass.Class)
		FirearmWidgetComponent->SetWidgetClass(FirearmWidgetClass.Class);
}

void AFirearmPickup::BeginPlay()
{
	Super::BeginPlay();
	
	InitPickup();
}

void AFirearmPickup::InitPickup()
{
	if (!Firearm)
		return;

	if (!Firearm->GetFirearmDefinition())
		return;

	FirearmMeshComponent->SetStaticMesh(Firearm->GetFirearmDefinition()->GetFirearmMesh());

	FirearmWidget = Cast<UFirearmDescriptionWidget>(FirearmWidgetComponent->GetWidget());

	if (FirearmWidget)
		FirearmWidget->InitFirearm(Firearm);
}

void AFirearmPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFirearmPickup, Firearm);
}

void AFirearmPickup::SetFirearm(UFirearm* NewFirearm)
{
	if (!NewFirearm)
		return;
	
	Firearm = NewFirearm;

	InitPickup();
}

void AFirearmPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirearmPickup::Interact(APlayerPawn* Pawn)
{
	if (!Pawn)
		return;

	Pawn->GetPlayerCombatComponent()->AddFirearm(Firearm);
	Destroy();
}

void AFirearmPickup::StartHovering()
{
	if (FirearmWidget)
		FirearmWidget->StartHovering();
}

void AFirearmPickup::StopHovering()
{
	if (FirearmWidget)
		FirearmWidget->StopHovering();
}

bool AFirearmPickup::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool wroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (!Firearm.IsNull()) 
	{
		wroteSomething |= Firearm->ReplicateSubobjects(Channel, Bunch, RepFlags);
		wroteSomething |= Channel->ReplicateSubobject(Firearm, *Bunch, *RepFlags);
	}

	return wroteSomething;
}

void AFirearmPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(AFirearmPickup, Firearm))
	{
		InitPickup();
	}
}

