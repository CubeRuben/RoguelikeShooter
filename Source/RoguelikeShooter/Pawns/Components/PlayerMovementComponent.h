#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "MovementStates/StateEnum.h"
#include "PlayerMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FMovementAttributes 
{
	GENERATED_USTRUCT_BODY();

public:

	FMovementAttributes() 
	{
		Acceleration = 15.0f;
		AccelerationInAir = 1.0f;
		StoppingAcceleration = 25.0f;
		MaxWalkingSpeed = 350.0f;
		MaxRunningSpeed = 650.0f;
		JumpForce = 500.0f;
		SlopeAngle = 50.0f;
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AccelerationInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StoppingAcceleration;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMovementState CurrentMovementStateType;

	class SBaseState* CurrentMovementState;
	TMap<EMovementState, class SBaseState*> StatesMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementAttributes MovementAttributes;

	// Replication
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FVector Location_Replicated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	FVector Velocity_Replicated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float PawnRotation_Replicated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	EMovementState CurrentMovementState_Replicated;

	void BeginPlay() override;
	void BeginDestroy() override;
		
	void AddStateToMap(class SBaseState* NewState);

	void HandleInput(float DeltaTime);

	void LocalPlayerTick(float DeltaTime);
	void ReplicatedPlayerTick(float DeltaTime);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	class APlayerPawn* GetPlayerPawn() const { return PlayerPawn; }
	const FMovementAttributes& GetMovementAttributes() const { return MovementAttributes; }

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetMovementInput();

	void SwitchToState(EMovementState NewState);

	bool CanStayOnSurface(FVector Normal);

	void Jump();

	void SweepGround(float Height, FHitResult& OutHit);

	UFUNCTION(Server, Unreliable)
	void ReplicateMovement_ServerRPC(FVector Location, FVector PawnVelocity, float PawnRotation, EMovementState MovementStateType);
	void ReplicateMovement_ServerRPC_Implementation(FVector Location, FVector PawnVelocity, float PawnRotation, EMovementState MovementStateType);

};
