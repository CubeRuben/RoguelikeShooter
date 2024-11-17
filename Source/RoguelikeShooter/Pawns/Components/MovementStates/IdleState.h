#pragma once

#include "BaseGroundedState.h"
#include "StateEnum.h"

class SIdleState : public SBaseGroundedState
{
public:
   
    SIdleState(class UPlayerMovementComponent* NewMovementComponent);

    EMovementState GetStateType() const override { return EMovementState::Idle; }

    bool UpdateInput() override;
    void UpdateVelocity(float DeltaTime) override;
};