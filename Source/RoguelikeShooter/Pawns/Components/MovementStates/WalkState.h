#pragma once

#include "BaseGroundedState.h"
#include "StateEnum.h"

class SWalkState : public SBaseGroundedState
{
public:

    SWalkState(class UPlayerMovementComponent* NewMovementComponent);

    EMovementState GetStateType() const override { return EMovementState::Walk; }

    bool UpdateInput() override;
    void UpdateVelocity(float DeltaTime) override;

    virtual float GetTargetSpeed();
};