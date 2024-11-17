#pragma once

#include "StateEnum.h"

class SBaseState 
{
public:

    SBaseState(class UPlayerMovementComponent* NewMovementComponent);

protected:

    class UPlayerMovementComponent* MovementComponent;

public:

    virtual EMovementState GetStateType() const = 0;

    virtual void OnStateEnter() {}
    virtual void OnStateExit() {}

    virtual void Tick(float DeltaTime) {}
};