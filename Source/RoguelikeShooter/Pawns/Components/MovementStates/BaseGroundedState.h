#pragma once

#include "BaseState.h"
#include "StateEnum.h"

class SBaseGroundedState : public SBaseState
{
public:

    SBaseGroundedState(class UPlayerMovementComponent* NewMovementComponent);

    void OnStateEnter() override;

    void Tick(float DeltaTime) override;

    virtual bool UpdateInput() = 0;
    virtual void UpdateVelocity(float DeltaTime) = 0;
};