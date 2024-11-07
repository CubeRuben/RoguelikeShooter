#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "PlayerMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FMovementAttributes 
{
	GENERATED_USTRUCT_BODY();

public:

	FMovementAttributes() 
	{
		Acceleration = 15.0f;
		MaxWalkingSpeed = 350.0f;
		MaxRunningSpeed = 650.0f;
		JumpForce = 500.0f;
		SlopeAngle = 50.0f;
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxWalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxRunningSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SlopeAngle;
};

UCLASS()
class ROGUELIKESHOOTER_API UPlayerMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
	
public:

	UPlayerMovementComponent();

protected:

	class APlayerPawn* PlayerPawn;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//FVector MovementVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FVector ReplicatedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float ReplicatedPawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementAttributes MovementAttributes;

	void BeginPlay() override;

	FVector GetMovementInput();
	float GetTargetSpeed();
		
	void HandleInput(float DeltaTime);

	void LocalPlayerTick(float DeltaTime);
	void ReplicatedPlayerTick(float DeltaTime);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Unreliable)
	void ReplicateMovement_ServerRPC(FVector Location, float PawnRotation);
	void ReplicateMovement_ServerRPC_Implementation(FVector Location, float PawnRotation);
};
