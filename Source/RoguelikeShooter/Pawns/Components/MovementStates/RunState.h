#pragma once

#include "WalkState.h"
#include "StateEnum.h"

class SRunState : public SWalkState
{
public:

    SRunState(class UPlayerMovementComponent* NewMovementComponent);

    EMovementState GetStateType() const override { return EMovementState::Run; }

    bool UpdateInput() override;

    virtual float GetTargetSpeed() override;
};