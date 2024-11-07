#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

struct FPlayerInput
{
	FPlayerInput() {}

	bool bMoveForward = false;
	bool bMoveLeft = false;
	bool bMoveBack = false;
	bool bMoveRight = false;

	bool bRun = false;
	bool bJump = false;

	float MouseX = 0.0f;
	float MouseY = 0.0f;
};

UCLASS()
class ROGUELIKESHOOTER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	APlayerPawn();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerMovementComponent* MovementComponent;

	FPlayerInput PlayerInput;

	virtual void BeginPlay() override;

public:	

	class UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
	class UCameraComponent* GetCameraComponent() const { return PlayerCamera; }

	FPlayerInput& GetPlayerInput() { return PlayerInput; }

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#define INPUT_HANDLE(name) void Input##name##Pressed() { PlayerInput.b##name = true; }; void Input##name##Released() { PlayerInput.b##name = false; };

	INPUT_HANDLE(MoveForward);
	INPUT_HANDLE(MoveLeft);
	INPUT_HANDLE(MoveBack);
	INPUT_HANDLE(MoveRight);

	INPUT_HANDLE(Run);
	INPUT_HANDLE(Jump);

	void InputMouseXAxis(float Value) { PlayerInput.MouseX = Value; };
	void InputMouseYAxis(float Value) { PlayerInput.MouseY = Value; };
};
