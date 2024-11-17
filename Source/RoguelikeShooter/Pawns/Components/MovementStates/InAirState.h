#pragma once

#include "BaseState.h"
#include "StateEnum.h"

class SInAirState : public SBaseState
{
public:

    SInAirState(class UPlayerMovementComponent* NewMovementComponent);

protected:

public:

    EMovementState GetStateType() const override { return EMovementState::InAir; }

    void OnStateExit() override;

    void Tick(float DeltaTime) override;
};